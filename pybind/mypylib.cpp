#include <pybind11/pybind11.h>

#include "mylib.hpp"

PYBIND11_MODULE(mylib, m) {
    m.doc() = "mylib"; // optional module docstring

    pybind11::class_<mylib::SimpleIO>(m, "SimpleIO")
        .def("getI", &mylib::SimpleIO::getI)
        .def("setI", &mylib::SimpleIO::setI)
        ;
        //int SimpleIO::getI() const{return i;};
        //void SimpleIO::setI(int newI){ i=newI;};
}
