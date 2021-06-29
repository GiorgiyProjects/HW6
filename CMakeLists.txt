cmake_minimum_required(VERSION 3.5)

project(bulk)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(bulk main.cpp)

set_target_properties(bulk PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)
