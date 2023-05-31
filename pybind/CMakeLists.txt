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

add_executable(doublePY doubleEmbed.cxx)
target_link_libraries(doublePY pybind11::embed)

find_package(OpenMP)
add_executable(doubleEmbedOMP doubleEmbedOMP.cxx)
target_link_libraries(doubleEmbedOMP pybind11::embed OpenMP::OpenMP_CXX)

find_package(MPI)
if (MPI_FOUND)
    add_executable(doubleEmbedMPI doubleEmbedMPI.cxx)
    target_link_libraries(doubleEmbedMPI pybind11::embed MPI::MPI_CXX)
endif (MPI_FOUND)