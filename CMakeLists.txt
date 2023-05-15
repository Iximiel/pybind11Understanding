cmake_minimum_required(VERSION 3.4...3.18)
#skeleton from https://github.com/9hack/cmake-skeleton
project(testingpy)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

# Compiler settings
set(CMAKE_CXX_STANDARD 11)
set(warnings "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

#pybind11
#find_package(pybind11 REQUIRED)

add_subdirectory(src)

add_subdirectory(test)