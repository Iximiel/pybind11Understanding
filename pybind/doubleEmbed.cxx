#include <iostream>
#include <pybind11/embed.h> // everything needed for embedding

namespace py = pybind11;
using namespace py::literals;
using std::cout;
#define CC std::cout << "Line: " << __LINE__ << "\n";

void func() {
  // py::initialize_interpreter();
  py::scoped_interpreter t{};

  auto locals = py::dict("j"_a = 6);
  py::exec(R"(
        i=5
        print(f"{i}")
        print(f"{j}")
    )",
           py::globals(), locals);
  locals["j"] = 8;
  py::exec(R"(
        print(f"{i}")
        print(f"{j}")
    )",
           py::globals(), locals);
  CC;

  CC;
  // py::finalize_interpreter();
  CC;
}

int main(int, char **) {
  try {
    CC;
    func();
    CC;
    // py::initialize_interpreter();
    py::scoped_interpreter s{};
    CC;
    py::exec(R"(
        print(f"asd")
    )");
    CC;
    // py::finalize_interpreter();
  } catch (pybind11::error_already_set &x) {
    cout << "pybind11::error_already_set\n";
  }
  return 0;
}
