// #define PYBIND11_DETAILED_ERROR_MESSAGES
#include <pybind11/embed.h> // everything needed for embedding
// #include <pybind11/numpy.h>
// #include <pybind11/pytypes.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

#include "mylib.hpp"

namespace py = pybind11;
using namespace pybind11::literals; // to bring in the `_a` literal

TEST_CASE("Playing with a dictionary", "[py]") {
  py::scoped_interpreter guard{};

  auto py_module = py::module::import("pylibtest");
  auto py_fcn = py_module.attr("setterdict");
  SECTION("Creating an empty dict and uploading a value to it") {
    py::dict d;
    auto i = GENERATE(take(100, random(-100, 100)));
    REQUIRE(d.size() == 0);
    py_fcn(d, "test", i);
    REQUIRE(d.contains("test"));
    REQUIRE(d["test"].cast<int>() == i);
  }
}

PYBIND11_EMBEDDED_MODULE(embeddedMylib, m) {
  m.doc() = "mylib"; // optional module docstring

  pybind11::class_<mylib::SimpleIO>(m, "SimpleIO")
      .def("getI", &mylib::SimpleIO::getI)
      .def("setI", &mylib::SimpleIO::setI);
}

TEST_CASE("Playing with embedded my lib", "[py]") {
  py::scoped_interpreter guard{};
  auto embeddedMylib = py::module_::import("embeddedMylib");
  auto py_module = py::module::import("pylibtest");
  auto py_fcn = py_module.attr("setter");

  SECTION("Creating and passing a SimpleIObject") {
    mylib::SimpleIO mytest;
    // auto mytest = embeddedMylib.attr("SimpleIO");
    // the name of the variables help reading the tests errors
    mytest.setI(42);
    int prevInCpp = mytest.getI();
    auto setValue = GENERATE(take(100, random(-100, 100)));
    auto prevInPy = py_fcn(mytest, setValue);
    REQUIRE(prevInPy.cast<int>() == prevInCpp);
    REQUIRE(mytest.getI() == setValue);
  }
}
