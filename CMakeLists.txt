cmake_minimum_required(VERSION 3.12.0)

string(TIMESTAMP DATE_TODAY "21%y%m%d")
set(PROJECT_VERSION 1.0.${DATE_TODAY})

project(iwd VERSION ${PROJECT_VERSION})

include(FetchContent)

FetchContent_Declare(
    get-argparse
    URL https://github.com/p-ranav/argparse/archive/v2.0.tar.gz
)

FetchContent_Declare(
    get-nlohmann-json
    URL https://github.com/nlohmann/json/archive/v3.7.3.tar.gz
)

FetchContent_MakeAvailable(
    get-argparse
    get-nlohmann-json
)

add_executable(
    iwd
    src/main.cpp)


set_target_properties(
    iwd
    PROPERTIES
    CXX_STANDARD 17
)

target_link_libraries(
    iwd
    PRIVATE
    argparse
    nlohmann_json
)
