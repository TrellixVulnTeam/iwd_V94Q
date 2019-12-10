#ifndef IWD__CMAKE_CONFIGURATION_HPP
#define IWD__CMAKE_CONFIGURATION_HPP

#include "iwd/quicktype/iwd.hpp"

#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

namespace iwd {

using quicktype_config_t =
  std::shared_ptr<std::map<std::string, nlohmann::json>>;

struct cmake_configuration
{
  cmake_configuration(quicktype_config_t initial_data);
  cmake_configuration(std::map<std::string, std::string> initial_data);

  static cmake_configuration from_arguments(
    const std::vector<std::string>& arguments);

  std::vector<std::string> as_cmake_args() const;

private:
  quicktype_config_t _configuration;
};

} // namespace iwd

#endif
