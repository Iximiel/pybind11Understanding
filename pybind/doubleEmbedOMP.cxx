#include <iostream>
#include <omp.h>
#include <pybind11/embed.h> // everything needed for embedding

namespace py = pybind11;
using namespace py::literals;
using std::cout;
#define CC std::cout << "Line: " << __LINE__ << "\n";

void func(int j) {
  CC;
  // py::initialize_interpreter();
  auto prc = std::to_string(j);
  auto locals = py::dict();
  locals[("j" + prc).c_str()] = j;
  py::exec(("print(f\"proc {j" + prc + "}\")").c_str(), py::globals(), locals);
}

int main(int, char **) {
  try {
    py::scoped_interpreter t{};
    omp_set_num_threads(2);
#pragma omp parallel
    {
      int j = omp_get_thread_num();
      std::cout << "c++proc" << j << "\n";
      func(j);
      CC;
    }
  } catch (pybind11::error_already_set &x) {
    cout << "pybind11::error_already_set\n";
  }
  return 0;
}

int main_() {
  py::scoped_interpreter guard{};

  auto locals = py::dict("name"_a = "World", "number"_a = 42);
  py::exec(R"(
        message = "Hello, {name}! The answer is {number}".format(**locals())
    )",
           py::globals(), locals);

  auto message = locals["message"].cast<std::string>();
  std::cout << message;
  return 0;
}