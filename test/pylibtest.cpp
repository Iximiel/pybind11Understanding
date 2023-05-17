// #define PYBIND11_DETAILED_ERROR_MESSAGES
#include <pybind11/embed.h> // everything needed for embedding
// #include <pybind11/numpy.h>
// #include <pybind11/pytypes.h>
#include <catch2/catch_template_test_macros.hpp>
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
  auto funcName = "setterdict";
  REQUIRE(py::hasattr(py_module, funcName));
  auto py_fcn = py_module.attr(funcName);
  SECTION("Creating an empty dict and uploading a value to it") {
    py::dict d;
    auto i = GENERATE(take(100, random(-100, 100)));
    REQUIRE(d.size() == 0);
    py_fcn(d, "test", i);
    REQUIRE(d.contains("test"));
    REQUIRE(d["test"].cast<int>() == i);
  }
}

TEMPLATE_TEST_CASE("Playing with a tuple and lists", "[py]", // py::tuple,
                   py::list) {
  py::scoped_interpreter guard{};

  auto py_module = py::module::import("pylibtest");
  auto funcName = "setterTupleLike";
  REQUIRE(py::hasattr(py_module, funcName));
  auto py_fcn = py_module.attr(funcName);
  SECTION("Creating the object and uploading two value to it") {
    // py::list d; // d{1000, 1000};
    TestType d(2);
    d[0] = py::int_(1000);
    d[1] = 1000;
    // REQUIRE(d[0].cast<int>() == 1000);
    // REQUIRE(d[1].cast<int>() == j);
    auto i = GENERATE(take(10, random(-100, 100)));
    auto j = GENERATE(take(10, random(-100, 100)));
    py_fcn(d, i, j);
    // REQUIRE(d[0].cast<int>() == i);
    // REQUIRE(d[1].cast<int>() == j);
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
        auto useParameterDirect =
            [fn = py_module.attr(funcName)](mylib::SimpleIO &simpleIO,
                                            int y) -> py::tuple {
          return fn(simpleIO, y).cast<py::tuple>();
        };
        constexpr int exampleValue = 42;
        auto setValue =
            GENERATE(take(100, filter([](int i) { return i != exampleValue; },
                                      random(-100, 100))));
        mylib::SimpleIO mytest;
        mytest.setI(exampleValue);
        // py::object obj = py::cast(&mytest);
        THEN("Setting and getting the values in python should not affect the "
             "C++ object") {
          // the name of the variables help reading the tests errors
          int prevInCpp = mytest.getI();
          auto retInPy = useParameterDirect(mytest, setValue);
          int prevInPy = retInPy[0].cast<int>();
          int setInPy = retInPy[1].cast<int>();
          REQUIRE(prevInPy == prevInCpp);
          REQUIRE(setInPy == setValue);
          // this will always fail
          CHECK_FALSE(mytest.getI() == setValue);
        }
      }
    }
  }
}

SCENARIO("checking parameters multiple times", "[py]") {
  GIVEN("A SimpleIO object") {
    constexpr int exampleValue = 42;
    mylib::SimpleIO mytest;
    AND_GIVEN("The python interpreter with the \"embeddedMylib\" loaded") {
      py::scoped_interpreter guard{};
      auto embeddedMylib = py::module_::import("embeddedMylib");
      AND_GIVEN("The pylibtest test module as an external .py file") {
        auto py_module = py::module::import("pylibtest");
        auto funcName = GENERATE("parameter", "getterSetter");
        THEN("The pylibtest has the attribute\"" << funcName << "\"") {
          REQUIRE(py::hasattr(py_module, funcName));
        }
        WHEN("We load a function that calls " << funcName) {
          auto useParameterObj = [fn = py_module.attr(funcName)](
                                     py::object &simpleIO, int y) -> py::tuple {
            return fn(simpleIO, y).cast<py::tuple>();
          };
          auto useParameterDirect =
              [fn = py_module.attr(funcName)](mylib::SimpleIO &simpleIO,
                                              int y) -> py::tuple {
            return fn(simpleIO, y).cast<py::tuple>();
          };
          auto setValue =
              GENERATE(take(100, filter([](int i) { return i != exampleValue; },
                                        random(-100, 100))));
          AND_WHEN("We cast the object as a type") {

            py::object obj = py::cast(&mytest);
            mytest.setI(exampleValue);
            THEN("Setting and getting the values in python should affect the "
                 "C++ "
                 "object") {
              // the name of the variables help reading the tests errors
              int prevInCpp = mytest.getI();
              // auto retInPy = useParameter(mytest, setValue);
              auto retInPy = useParameterObj(obj, setValue);

              int prevInPy = retInPy[0].cast<int>();
              int setInPy = retInPy[1].cast<int>();
              REQUIRE(prevInPy == prevInCpp);
              REQUIRE(setInPy == setValue);
              REQUIRE(mytest.getI() == setValue);
            }

            AND_WHEN("We use the object direclty") {
              mylib::SimpleIO myOthertest;
              myOthertest.setI(exampleValue);
              THEN("Setting and getting the values in python should not affect "
                   "the "
                   "C++ object") {
                // the name of the variables help reading the tests errors
                int prevInCpp = myOthertest.getI();
                auto retInPy = useParameterDirect(myOthertest, setValue);
                int prevInPy = retInPy[0].cast<int>();
                int setInPy = retInPy[1].cast<int>();
                REQUIRE(prevInPy == prevInCpp);
                REQUIRE(setInPy == setValue);
                // It is better to check a single value:
                // CHECK_FALSE(myOthertest.getI() == setValue);
                CHECK(myOthertest.getI() == prevInCpp);
              }
            }
          }
        }
      }
    }
    THEN("Even if the interpreter is out of, scope the variable mytest should "
         "still "
         "exist") {
      REQUIRE(mytest.getI() != exampleValue);
    }
  }
}