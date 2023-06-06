
add_library(mylib mylib.cpp)

set_target_properties(mylib PROPERTIES 
    PUBLIC_HEADER mylib.hpp)

target_include_directories(mylib PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src>"
    "$<INSTALL_INTERFACE:include>"
    )

add_library(MYLIB::mylib ALIAS mylib)

install(TARGETS mylib
        EXPORT mylibTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}ls
)

if (NOT SKBUILD)
    include(CMakePackageConfigHelpers)

    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/mylibConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mylib
    )

    write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/mylibConfigVersion.cmake"
    VERSION "0.0.1"
    COMPATIBILITY AnyNewerVersion
    )

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/mylibConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/mylibConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mylib
    )

    install(EXPORT mylibTargets
            FILE mylibTargets.cmake
            NAMESPACE MYLIB::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mylib
    )

    # export(EXPORT mylibTargets
    #        FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/mylibTargets.cmake"
    #        NAMESPACE MYLIB::
    #        #DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mylib
    # )
endif (NOT SKBUILD)