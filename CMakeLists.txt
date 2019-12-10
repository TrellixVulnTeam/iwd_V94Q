cmake_minimum_required(VERSION 3.12.0)

string(TIMESTAMP DATE_TODAY "21%y%m%d")
set(PROJECT_VERSION 1.0.${DATE_TODAY})

project(iwd VERSION ${PROJECT_VERSION})

add_executable(
    iwd
    src/main.cpp)


set_target_properties(
    iwd
    PROPERTIES
    CXX_STANDARD 17
)
