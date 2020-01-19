#include "iwd/cmake_configuration.hpp"

#include <iomanip>
#include <regex>
#include <vn/string_template.hpp>
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

vn::string_template::value_provider_functor
make_value_provider(const cmake_configuration& config)
{
  return [&config](const std::string& key) -> std::optional<std::string> {
    return config.get_argument(key);
  };
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
}

std::optional<std::string>
cmake_configuration::get_argument(const std::string& key) const
{
  auto it = _configuration->find(key);
  if (it != _configuration->end()) {
    return it->second.get<std::string>();
  }
  return std::nullopt;
}

cmake_configuration
cmake_configuration::override_with(const cmake_configuration& parent) const
{
  cmake_configuration result(nullptr);
  auto value_provider = make_value_provider(parent);

  for (const auto& [k, v] : *_configuration) {

    std::smatch match;
    auto override_value = parent.get_argument(k);

    if (override_value) {
      // Set argument value from parent
      result.set_argument(k, *override_value);
    } else {
      // Set argument from current config, and try to override all $(VARS) with
      // parent config
      result.set_argument(
        k, vn::render_template(v.get<std::string>(), value_provider));
    }
  }

  for (const auto& [k, v] : parent.configuration_copy()) {
    result.set_argument(k, v.get<std::string>());
  }
  return result;
}

std::optional<nlohmann::json>
cmake_configuration::get_argument_obj(const std::string& key) const
{
  auto it = _configuration->find(key);
  if (it != _configuration->end()) {
    return it->second;
  }
  return std::nullopt;
}

void
cmake_configuration::set_argument(
  const std::string& key,
  const std::string& value)
{
  _configuration->insert(std::make_pair(key, nlohmann::json(value)));
}

quicktype_config_t::element_type
cmake_configuration::configuration_copy() const
{
  return *_configuration;
}

} // namespace iwd
