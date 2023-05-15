# Understanding some tricks for pybind11

This is a toy repo for checking how is easy is sending  data back and forth between
C++ and python.
I am using pybind11 and catch2 for tests (the main executable here are the tests)

If pybind11 or catch2 are not found on your system they are automatically
downloaded with CMake's `FetchContent`.


kickstart:
```bash
mkdir build ; cd build
cmake ../ . -DCMAKE_BUILD_TYPE="Release"
cmake --build.
```
then run the tests (from the build folder)
```
cd test
ctest
```