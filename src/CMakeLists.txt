
add_library(mylib mylib.cpp)

set_target_properties(mylib PROPERTIES 
    PUBLIC_HEADER mylib.hpp)

target_include_directories(mylib PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src>"
    "$<INSTALL_INTERFACE:include>"
    )

add_library(MYLIB::MYLIB ALIAS mylib)