cmake_minimum_required(VERSION 3.5)

project(bulk)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(bulk main.cpp CommandBlock.h)
add_executable(test tests.cpp CommandBlock.h)

set_target_properties(bulk test PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
