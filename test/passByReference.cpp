#include <fstream> //used to have everithing in one file

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

#include <pybind11/embed.h>

#include "mylib.hpp"

namespace py = pybind11;

constexpr auto externalModule = R"(
import embeddedMylib
def parameter(objectToSet:embeddedMylib.SimpleIO, value:int):
    toret = objectToSet.I
    objectToSet.I = value
    toret2 = objectToSet.I
    return toret, toret2

def getterSetter(objectToSet:embeddedMylib.SimpleIO, value:int):
    toret = objectToSet.getI()
    objectToSet.setI(value)
    toret2 = objectToSet.getI()
    return toret, toret2

)";

// this fails with "return_value_policy = copy, but type is non-copyable!"
// when the copy constructor of SimpleIO is deleted
SCENARIO("Directly pass the C++ object") {
  GIVEN("The C++ object") {
    // this is here to make sure that the py::scoped_interpreter goes out of
    // scope
    mylib::SimpleIO mytest;
    // set to not overlap
    auto exampleValue = GENERATE(take(10, random(0, 100)));
    auto setValue = GENERATE(take(10, random(1000, 1100)));
    AND_GIVEN(
        "The embedded interpreter the interface module and a function that "
        "uses the module") {
      {
        std::ofstream f("passbyReference.py");
        f << externalModule;
      }
      py::scoped_interpreter guard{};
      auto embeddedMylib = py::module_::import("embeddedMylib");
      auto py_module = py::module::import("passbyReference");
      auto funcName = GENERATE("parameter", "getterSetter");
      WHEN("We load a function that calls " << funcName) {
        auto useParameterDirect =
            [fn = py_module.attr(funcName)](mylib::SimpleIO &simpleIO,
                                            int y) -> py::tuple {
          return fn(simpleIO, y).cast<py::tuple>();
        };
        mytest.setI(exampleValue);
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
          // CHECK_FALSE(mytest.getI() == setValue);
          // better test that nothing is changed
          CHECK(mytest.getI() == exampleValue);
        }
      }
    }

    // WHEN("The interpreter goes out of scope") {
    // THEN("The C+ object should survive") {
    CHECK(mytest.getI() == exampleValue);
    //}
    //}
  }
}

SCENARIO("Cast the C++ objet to a python object") {
  GIVEN("The C++ object") {
    // this is here to make sure that the py::scoped_interpreter goes out of
    // scope
    mylib::SimpleIO mytest;
    // set to not overlap
    auto exampleValue = GENERATE(take(10, random(0, 100)));
    auto setValue = GENERATE(take(10, random(1000, 1100)));
    AND_GIVEN(
        "The embedded interpreter the interface module and a function that "
        "uses the module") {
      {
        std::ofstream f("passbyReference.py");
        f << externalModule;
      }
      py::scoped_interpreter guard{};
      auto embeddedMylib = py::module_::import("embeddedMylib");
      auto py_module = py::module::import("passbyReference");
      auto funcName = GENERATE("parameter", "getterSetter");
      WHEN("We load a function that calls " << funcName) {
        auto useParameterDirect =
            [fn = py_module.attr(funcName)](mylib::SimpleIO &simpleIO,
                                            int y) -> py::tuple {
          return fn(simpleIO, y).cast<py::tuple>();
        };
        mytest.setI(exampleValue);
        // I don't know why this is working!!!
        // we cast to python
        auto obj = py::cast(&mytest);
        THEN("Setting and getting the values in python should not affect the "
             "C++ object") {
          // the name of the variables help reading the tests errors
          int prevInCpp = mytest.getI();
          auto retInPy = useParameterDirect(mytest, setValue);
          int prevInPy = retInPy[0].cast<int>();
          int setInPy = retInPy[1].cast<int>();
          REQUIRE(prevInPy == prevInCpp);
          REQUIRE(setInPy == setValue);

          CHECK(mytest.getI() == setValue);
        }
      }
    }
    // WHEN("The interpreter goes out of scope") {
    // THEN("The C+ object should survive") {
    // the set happened in python:
    CHECK(mytest.getI() == setValue);
    // }
    // }
  }
}

SCENARIO("Or, from the award winning series \"know what you are doing\":pass "
         "the pointer") {
  GIVEN("The C++ object") {
    // this is here to make sure that the py::scoped_interpreter goes out of
    // scope
    mylib::SimpleIO mytest;
    // set to not overlap
    auto exampleValue = GENERATE(take(10, random(0, 100)));
    auto setValue = GENERATE(take(10, random(1000, 1100)));
    AND_GIVEN(
        "The embedded interpreter the interface module and a function that "
        "uses the module") {
      {
        std::ofstream f("passbyReference.py");
        f << externalModule;
      }
      py::scoped_interpreter guard{};
      auto embeddedMylib = py::module_::import("embeddedMylib");
      auto py_module = py::module::import("passbyReference");
      auto funcName = GENERATE("parameter", "getterSetter");
      WHEN("We load a function that calls " << funcName) {
        auto useParameterDirect =
            [fn = py_module.attr(funcName)](mylib::SimpleIO *simpleIO,
                                            int y) -> py::tuple {
          return fn(simpleIO, y).cast<py::tuple>();
        };
        mytest.setI(exampleValue);
        THEN("Setting and getting the values in python should not affect the "
             "C++ object") {
          // the name of the variables help reading the tests errors
          int prevInCpp = mytest.getI();
          auto retInPy = useParameterDirect(&mytest, setValue);
          int prevInPy = retInPy[0].cast<int>();
          int setInPy = retInPy[1].cast<int>();
          REQUIRE(prevInPy == prevInCpp);
          REQUIRE(setInPy == setValue);

          CHECK(mytest.getI() == setValue);
        }
      }
    }
    // WHEN("The interpreter goes out of scope") {
    // THEN("The C+ object should survive") {
    // the set happened in python:
    CHECK(mytest.getI() == setValue);
    // }
    // }
  }
}