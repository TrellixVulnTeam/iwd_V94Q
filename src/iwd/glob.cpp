#include "iwd/glob.hpp"

#include "iwd/fnmatch.hpp"
#include <algorithm>
#include <vn/string_utils.hpp>

namespace iwd {

namespace detail {

namespace {

std::regex magic_check = std::regex(R"([*?[])");
bool
has_magic(std::string_view str)
{
  std::cmatch match;
  return std::regex_search(str.begin(), str.end(), match, magic_check);
}

bool
is_hidden(std::string_view fname)
{
  return fname.size() > 0 && fname[0] == '.';
}

void
iglob(const std::filesystem::path& path)
{
  const auto dirname = path.parent_path();
  const auto basename = path.filename();

  if (!has_magic(path.c_str())) {
    if (!basename.empty()) {
      if (std::filesystem::exists(basename)) {
        // yield pathname
      }
    } else {
      if (std::filesystem::is_directory(dirname)) {
        // yield pathname
      }
    }
    return;
  }

  if (dirname.empty()) {
  }
}

std::vector<std::string>
glob1(const vn::directory& dir, std::string_view pattern, bool dironly)
{
  iwd::fnmatch fnmatch(pattern);

  std::vector<std::string> result;
  const auto file_list = dir.list();

  for (const auto& entry : dir.list()) {
    if (
      dironly && std::filesystem::is_directory(entry) &&
      std::invoke(fnmatch, entry)) {
      result.push_back(entry.path().filename().string());
    }

    else if (!dironly)
  }
}

} // namespace

globbing_predicate::globbing_predicate(
  const std::filesystem::path& root_path,
  const std::string& glob_pattern)
  : _root_path(root_path)
  , _directory_pattern()
  , _file_pattern()
{
  const auto translated = translate_glob_expr(glob_pattern);
  // Split the path part into filename, and directory name
  const auto split = translated.find_last_of("/");
  const auto tr_view = std::string_view(translated);
  const auto basename =
    split == std::string::npos ? tr_view : tr_view.substr(split + 1u);
  const auto dirname =
    split == std::string::npos ? std::string_view() : tr_view.substr(0, split);

  _directory_pattern = std::regex(std::string(dirname));
  _file_pattern = std::regex(std::string(basename));
}

bool
globbing_predicate::operator()(
  const std::filesystem::directory_entry& entry) const
{
  if (!std::filesystem::is_directory(entry)) {
    const auto relative_path =
      std::filesystem::relative(entry.path(), _root_path);

    const auto dirname = relative_path.parent_path();
    const auto fname = relative_path.filename();

    [[maybe_unused]] const auto fname_match = matches(_file_pattern, fname);
    [[maybe_unused]] const auto dirname_match =
      matches(_directory_pattern, dirname);

    return matches(_file_pattern, fname) &&
      matches(_directory_pattern, dirname);
  }
  return false;
}

} // namespace detail

namespace {

recursive_glob_iterator
make_filter_iterator(
  const std::filesystem::path& root_path,
  const std::string& glob_expr,
  std::filesystem::recursive_directory_iterator iterator)
{
  return recursive_glob_iterator(
    detail::globbing_predicate(root_path, glob_expr),
    iterator,
    std::filesystem::recursive_directory_iterator());
}

} // namespace

vn::iterator_range<recursive_glob_iterator>
glob_recursive(const vn::directory& in_directory, const std::string& pattern)
{
  auto all_elements = in_directory.walk();

  return vn::make_iterator_range(
    make_filter_iterator(in_directory.path(), pattern, all_elements.begin()),
    make_filter_iterator(in_directory.path(), pattern, all_elements.end()));
}

} // namespace iwd
