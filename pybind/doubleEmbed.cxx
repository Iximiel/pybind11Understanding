#include <iostream>
#include <memory>
#include <pybind11/embed.h> // everything needed for embedding

namespace py = pybind11;
using namespace py::literals;
using std::cout;
#define CC std::cout << "Line: " << __LINE__ << "\n";

class myScopedInterpreter {
private:
  static unsigned counter;
  static std::unique_ptr<py::scoped_interpreter> interpreter;

public:
  myScopedInterpreter() {
    if (counter == 0 && !Py_IsInitialized()) {
      interpreter = std::make_unique<py::scoped_interpreter>();
    }
    ++counter;
  }
  ~myScopedInterpreter() {
    --counter;
    if (counter == 0) {
      // in case this will release a nullptr
      interpreter.reset(nullptr);
    }
  }
};

unsigned myScopedInterpreter::counter = 0;
std::unique_ptr<py::scoped_interpreter> myScopedInterpreter::interpreter =
    nullptr;

void secondfunc() {
  // py::initialize_interpreter();
  myScopedInterpreter t{};

  auto locals = py::dict("j"_a = 77);
  py::exec(R"(
        print(f"second")
    )",
           py::globals(), locals);
  locals["j"] = 88;
  py::exec(R"(
        print(f"{j}")
    )",
           py::globals(), locals);
}

void func() {
  // py::initialize_interpreter();
  myScopedInterpreter t{};

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
  secondfunc();
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
