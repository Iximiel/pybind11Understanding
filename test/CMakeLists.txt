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
if(NOT TARGET MYLIB::mylib)
    find_package(MYLIB REQUIRED)
endif()

add_executable(mypybindtests baselibtest.cpp)
target_link_libraries(mypybindtests PRIVATE
    Catch2::Catch2WithMain MYLIB::mylib)

add_executable(mypybindtestswithPY pylibtest.cpp)
target_link_libraries(mypybindtestswithPY PRIVATE
    Catch2::Catch2WithMain MYLIB::mylib pybind11::embed)

add_executable(passByReferenceTest passByReference.cpp)
target_link_libraries(passByReferenceTest PRIVATE
    Catch2::Catch2WithMain
    #pybind11::embed
    #MYLIB::MYLIB
    #this tests that the embedded module can be distribuited externally!!!!
    MYLIB::PYEMBEDDEDMYLIB)

catch_discover_tests(mypybindtests)
catch_discover_tests(mypybindtestswithPY
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )

catch_discover_tests(passByReferenceTest)

find_package(Python COMPONENTS Interpreter REQUIRED)
add_test(NAME foolishADD
    COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/test_foolish.py
)
set_property(TEST foolishADD PROPERTY ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_BIN_DIR}../pybindmodule:$ENV{PYTHONPATH}")

add_test(NAME mypylibTest
    COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/test_myLib.py
)
set_property(TEST mypylibTest PROPERTY ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_BIN_DIR}../pybindmodule:$ENV{PYTHONPATH}")