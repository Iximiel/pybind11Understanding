cmake_minimum_required(VERSION 3.15...3.22)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG QUIET)
if (NOT pybind11_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        pybind11
  GIT_SHALLOW    TRUE
  GIT_REPOSITORY https://github.com/pybind/pybind11
  GIT_TAG        v2.10.4
  )
    FetchContent_MakeAvailable(pybind11)
 #   find_package(pybind11 CONFIG )
endif(NOT pybind11_FOUND)

#include(pybind11)
#this should make the tests standalone
if(NOT TARGET MYLIB::MYLIB)
    find_package(MYLIB REQUIRED)
endif()

add_library(mypylibEmbedded OBJECT mypylibEmbedded.cpp)


target_link_libraries(mypylibEmbedded
    PUBLIC 
        pybind11::embed
        MYLIB::MYLIB)
add_library(MYLIB::PYEMBEDDEDMYLIB ALIAS mypylibEmbedded)

add_library(mypylib MODULE mypylib.cpp)

target_link_libraries(mypylib PUBLIC pybind11::module)
target_link_libraries(mypylib PUBLIC MYLIB::MYLIB)

function(print_target_property target_name property)
    get_target_property(tempvar ${target_name} ${property})
    message("${target_name} <${property}>: ${tempvar}")
    unset(tempvar)    
endfunction(print_target_property)
print_target_property(mypylib INCLUDE_DIRECTORIES)
print_target_property(MYLIB::MYLIB INTERFACE_INCLUDE_DIRECTORIES)
pybind11_extension(mypylib)
if(NOT MSVC AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug|RelWithDebInfo)
    # Strip unnecessary sections of the binary on Linux/macOS
    pybind11_strip(mypylib)
endif()

install(TARGETS mypylib LIBRARY DESTINATION mylib)

set_target_properties(mypylib PROPERTIES
                    INTERPROCEDURAL_OPTIMIZATION ON
                    CXX_VISIBILITY_PRESET "default"
                    VISIBLITY_INLINES_HIDDEN ON)