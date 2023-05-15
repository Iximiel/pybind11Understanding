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
