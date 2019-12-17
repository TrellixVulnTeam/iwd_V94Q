#ifndef IWD__GLOB_HPP
#define IWD__GLOB_HPP

#include <boost/iterator/filter_iterator.hpp>
#include <regex>
#include <vn/directory.hpp>
#include <vn/iterator_range.hpp>

namespace iwd {

namespace detail {
struct globbing_predicate
{
  globbing_predicate(
    const std::filesystem::path& root_path,
    const std::string& glob_pattern);
  bool operator()(const std::filesystem::directory_entry& entry) const;

private:
  std::filesystem::path _root_path;
  std::regex _directory_pattern;
  std::regex _file_pattern;
};
} // namespace detail

using recursive_glob_iterator = boost::filter_iterator<
  detail::globbing_predicate,
  std::filesystem::recursive_directory_iterator>;

vn::iterator_range<recursive_glob_iterator>
glob_recursive(const vn::directory& in_directory, const std::string& pattern);
} // namespace iwd

#endif
