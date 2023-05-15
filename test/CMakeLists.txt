cmake_minimum_required(VERSION 3.16)
project(MYLIBTests LANGUAGES CXX)

find_package(Catch2 QUIET)
if (NOT Catch2_FOUND)
    include(FetchContent)
    FetchContent_Declare(
  Catch2
  GIT_SHALLOW    TRUE
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.3.2)
    FetchContent_MakeAvailable(Catch2)
endif(NOT Catch2_FOUND)

# For catch 2 only
# list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)
# For catch 3 only
#list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

enable_testing()

#this should make the tests standalone
if(NOT TARGET MYLIB::MYLIB)
    find_package(BH++ REQUIRED)
    set(TestWDIR ${BH++_EXAMPLES_DIR})
endif()

add_executable(mypybindtests baselibtest.cpp)
target_link_libraries(mypybindtests PRIVATE Catch2::Catch2WithMain MYLIB::MYLIB)

catch_discover_tests(mypybindtests)