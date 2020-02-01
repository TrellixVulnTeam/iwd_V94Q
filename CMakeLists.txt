cmake_minimum_required(VERSION 3.12.0)

string(TIMESTAMP DATE_TODAY "21%y%m%d")
set(PROJECT_VERSION 1.0.${DATE_TODAY})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(THREADS_PREFER_PTHREAD_FLAG On)

project(iwd VERSION ${PROJECT_VERSION})

include(FetchContent)

set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
set(CATCH_BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    get-argparse
    URL https://github.com/p-ranav/argparse/archive/v2.0.tar.gz
)

FetchContent_Declare(
    get-nlohmann-json
    URL https://github.com/nlohmann/json/archive/v3.7.3.tar.gz
)

FetchContent_Declare(
    get-libarchive
    URL https://www.libarchive.org/downloads/libarchive-3.4.0.tar.gz
)

FetchContent_Declare(
    get-spdlog
    URL https://github.com/gabime/spdlog/archive/v1.4.2.tar.gz
)

FetchContent_Declare(
    get-catch2
    URL https://github.com/catchorg/Catch2/archive/v2.11.1.tar.gz
)

FetchContent_MakeAvailable(
    get-argparse
    get-nlohmann-json
    get-spdlog
    get-catch2
)

find_package(LibArchive REQUIRED)
find_package(Boost 1.71.0 REQUIRED COMPONENTS system filesystem)
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)

add_library(
    vn
    src/vn/directory.cpp
    src/vn/file.cpp
    src/vn/fnmatch.cpp
    src/vn/string_utils.cpp
    src/vn/path.cpp
    src/vn/glob.cpp
    src/vn/string_template.cpp
)

add_executable(
    iwd
    src/main.cpp
    src/iwd/check_call.cpp
    src/iwd/cmake_configuration.cpp
    src/iwd/directories.cpp
    src/iwd/download_file.cpp
    src/iwd/executable.cpp
    src/iwd/extract_tarfile.cpp
    src/iwd/git_clone.cpp
    src/iwd/parse_args.cpp
    src/iwd/requirements.cpp
)


set_target_properties(
    iwd
    vn
    PROPERTIES
    CXX_STANDARD 17
)

target_link_libraries(
    iwd
    PRIVATE
    vn
    argparse
    nlohmann_json
    ${LibArchive_LIBRARIES}
    Boost::system
    Boost::filesystem
    Threads::Threads
    CURL::libcurl
    spdlog
)

target_include_directories(
    vn
    PUBLIC
    src
)

target_include_directories(
    iwd
    PRIVATE
    src/
    ${LibArchive_INCLUDE_DIRS}
)

enable_testing()
include(CTest)
include(${get-catch2_SOURCE_DIR}/contrib/ParseAndAddCatchTests.cmake)

add_executable(
    vn-tests
    test/vn/main.cpp
    test/vn/fnmatch.cpp
    test/vn/path.cpp
    test/vn/string_template.cpp
)

target_link_libraries(
    vn-tests
    PRIVATE
        vn
        Catch2::Catch2
)

set_target_properties(
    vn-tests
    PROPERTIES
    CXX_STANDARD 17
)

ParseAndAddCatchTests(vn-tests)
