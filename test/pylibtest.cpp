// #define PYBIND11_DETAILED_ERROR_MESSAGES
#include <pybind11/embed.h> // everything needed for embedding
// #include <pybind11/numpy.h>
// #include <pybind11/pytypes.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

#include <iostream>
#define CC std::cout << __LINE__ << "\n";

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
      .def(py::init<>())
      .def("getI", &mylib::SimpleIO::getI)
      .def("setI", &mylib::SimpleIO::setI)
      .def_property("I", &mylib::SimpleIO::getI, &mylib::SimpleIO::setI,
                    py::return_value_policy::copy);
}

SCENARIO("Playing with embeddedMylib", "[py]") {
  GIVEN("The python interpreter with the \"embeddedMylib\" loaded") {
    py::scoped_interpreter guard{};
    auto embeddedMylib = py::module_::import("embeddedMylib");
    AND_GIVEN("The pylibtest test module as an external .py file") {
      auto py_module = py::module::import("pylibtest");
      auto funcName = GENERATE("parameter", "getterSetter");
      THEN("The pylibtest has the attribute\"" << funcName << "\"") {
        REQUIRE(py::hasattr(py_module, funcName));
      }
      WHEN("We load a function that calls " << funcName) {
        auto useParameter = [fn = py_module.attr(funcName)](
                                mylib::SimpleIO &simpleIO, int y) -> py::tuple {
          return fn(simpleIO, y).cast<py::tuple>();
        };
        constexpr int exampleValue = 42;
        auto setValue =
            GENERATE(take(100, filter([](int i) { return i != exampleValue; },
                                      random(-100, 100))));
        mylib::SimpleIO mytest;
        mytest.setI(exampleValue);
        THEN("Setting and getting the values in python should work") {
          // the name of the variables help reading the tests errors
          int prevInCpp = mytest.getI();
          auto retInPy = useParameter(mytest, setValue);
          int prevInPy = retInPy[0].cast<int>();
          int setInPy = retInPy[1].cast<int>();
          REQUIRE(prevInPy == prevInCpp);
          REQUIRE(setInPy == setValue);
          // this will always fail
          CHECK(mytest.getI() == setValue);
        }
      }
    }
  }
}
