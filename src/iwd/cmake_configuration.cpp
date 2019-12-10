#include "iwd/cmake_configuration.hpp"

#include <iomanip>
#include <regex>
#include <vn/string_utils.hpp>

namespace iwd {

namespace {

const auto argument_regex =
  std::regex(R"__regex__((?:-D)?(\w+)=([\w\s/]+))__regex__");

}

namespace {
std::string
make_cmake_arg(const std::string& key, const std::string& value)
{
  return "-D" + key + "=" + value;
}

} // namespace

cmake_configuration::cmake_configuration(quicktype_config_t initial_data)
  : _configuration(
      initial_data ? initial_data
                   : std::make_shared<std::map<std::string, nlohmann::json>>())
{}

cmake_configuration::cmake_configuration(
  std::map<std::string, std::string> initial_data)
  : cmake_configuration(nullptr)
{
  for (const auto& pair : initial_data) {
    (*_configuration)[pair.first] = pair.second;
  }
}

std::vector<std::string>
cmake_configuration::as_cmake_args() const
{
  std::vector<std::string> result;
  result.reserve(_configuration->size());
  for (const auto& pair : (*_configuration)) {
    result.push_back(make_cmake_arg(pair.first, pair.second));
  }
  return result;
}

cmake_configuration
cmake_configuration::from_arguments(const std::vector<std::string>& arguments)
{
  auto config = cmake_configuration(nullptr);
  std::smatch match;
  for (const auto& arg : arguments) {
    if (!std::regex_match(arg, match, argument_regex)) {
      throw std::runtime_error(vn::make_message(
        std::string("Invalid cmake argument passed"), std::quoted(arg)));
    }
    const auto name = match[1].str();
    const auto value = match[2].str();
    config._configuration->insert_or_assign(name, value);
  }
  return config;
} // namespace iwd

} // namespace iwd
