#include "iwd/extract_tarfile.hpp"

#include <archive.h>
#include <archive_entry.h>
#include <memory>
#include <vector>
#include <vn/string_utils.hpp>

namespace iwd {

namespace {
constexpr auto archive_extract_flags =
  ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS;

struct archive_read_deleter
{
  void operator()(archive* ar)
  {
    archive_read_close(ar);
    archive_read_free(ar);
  }
};

struct archive_write_deleter
{
  void operator()(archive* ar)
  {
    archive_write_close(ar);
    archive_write_free(ar);
  }
};

using read_archive_uptr = std::unique_ptr<archive, archive_read_deleter>;
using write_archive_uptr = std::unique_ptr<archive, archive_write_deleter>;

constexpr auto block_size = 10240;

read_archive_uptr
make_read_archive(const std::filesystem::path& file_path, int block_size)
{
  read_archive_uptr ar(archive_read_new());
  archive_read_support_format_all(ar.get());
  archive_read_support_filter_all(ar.get());
  if (int r =
        archive_read_open_filename(ar.get(), file_path.c_str(), block_size);
      r != ARCHIVE_OK) {
    const auto err = std::string(archive_error_string(ar.get()));
    throw std::runtime_error(vn::make_message(
      "Could not open archive",
      std::quoted(file_path.string()),
      "for reading. Reason:",
      std::quoted(err)));
  }
  return ar;
}

write_archive_uptr
make_write_archive()
{
  write_archive_uptr ar(archive_write_disk_new());
  archive_write_disk_set_options(ar.get(), archive_extract_flags);
  return ar;
}

void
throw_archive_error(const std::filesystem::path& tar_location, archive* ar)
{
  const auto err_s = std::string(archive_error_string(ar));
  throw std::runtime_error(vn::make_message(
    "Failed while reading entry of archive:",
    std::quoted(tar_location.string()),
    "Reason:",
    std::quoted(err_s)));
}

int
copy_data(archive* source, archive* destination)
{
  int r;
  const void* buff;
  size_t size;
  la_int64_t offset;

  for (;;) {
    r = archive_read_data_block(source, &buff, &size, &offset);
    if (r == ARCHIVE_EOF) {
      return ARCHIVE_OK;
    }
    if (r < ARCHIVE_OK) {
      return r;
    }
    r = archive_write_data_block(destination, buff, size, offset);
    if (r < ARCHIVE_OK) {
      throw std::runtime_error(vn::make_message(
        "Error occurred while extracting archive",
        std::quoted(std::string(archive_error_string(destination)))));
    }
  }
}

void
write_header_or_throw(archive* archive, archive_entry* entry)
{
  if (const auto error = archive_write_header(archive, entry);
      error < ARCHIVE_OK) {
    throw std::runtime_error(std::string(archive_error_string(archive)));
  }
}

void
finish_entry_or_throw(archive* archive)
{
  if (const auto error = archive_write_finish_entry(archive);
      error < ARCHIVE_OK) {
    throw std::runtime_error(std::string(archive_error_string(archive)));
  }
}

} // namespace

void
extract_tarfile(
  const std::filesystem::path& tar_location,
  const vn::directory& extract_location,
  std::optional<tar_entry_visitor_t> entry_visitor)
{
  if (!entry_visitor) {
    entry_visitor = [](tar_entry&) { return true; };
  }
  auto read = make_read_archive(tar_location, block_size);
  auto write = make_write_archive();
  archive_entry* entry = nullptr;

  while (true) {
    const auto err = archive_read_next_header(read.get(), &entry);
    if (err == ARCHIVE_EOF) {
      break;
    }

    if (err != ARCHIVE_OK) {
      throw_archive_error(tar_location, read.get());
    }

    tar_entry tar_entry = { std::filesystem::path(
      archive_entry_pathname(entry)) };
    const auto do_copy = std::invoke(*entry_visitor, tar_entry);
    if (do_copy) {
      tar_entry.file_path = extract_location.path() / tar_entry.file_path;
      archive_entry_set_pathname(entry, tar_entry.file_path.c_str());
      write_header_or_throw(write.get(), entry);
      copy_data(read.get(), write.get());
      finish_entry_or_throw(write.get());
    }
  }
}

} // namespace iwd
