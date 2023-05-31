#include "mylib.hpp"
#include <pybind11/embed.h>
namespace py = pybind11;
PYBIND11_EMBEDDED_MODULE(embeddedMylib, m) {
  m.doc() = "mylib"; // optional module docstring

  pybind11::class_<mylib::SimpleIO>(m, "SimpleIO")
      .def(py::init<>())
      .def("getI",
           &mylib::SimpleIO::getI) //, py::return_value_policy::reference)
      .def("setI",
           &mylib::SimpleIO::setI) //, py::return_value_policy::reference)
      .def_property("I", &mylib::SimpleIO::getI, &mylib::SimpleIO::setI
                    //,py::return_value_policy::reference
      );
}
