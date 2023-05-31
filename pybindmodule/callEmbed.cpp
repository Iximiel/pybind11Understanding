#include <mutex>
#include <pybind11/embed.h> // everything needed for embedding
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace pybind11::literals; // to bring in the `_a` literal

class myScopedInterpreter {
private:
  static unsigned counter;
  static std::unique_ptr<py::scoped_interpreter> interpreter;
  static std::mutex interpreterMutex;

public:
  myScopedInterpreter() {
    std::lock_guard<std::mutex> lk(interpreterMutex);
    if (counter == 0 && Py_IsInitialized()) {
      ++counter; // check if makint this is safe enough
      // is it safer to turn back to a if (counter == 0 && !Py_IsInitialized())
      // approach?
    }
    if (counter == 0) {
      interpreter = std::make_unique<py::scoped_interpreter>();
    }
    ++counter;
  }
  ~myScopedInterpreter() {
    std::lock_guard<std::mutex> lk(interpreterMutex);
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
std::mutex myScopedInterpreter::interpreterMutex;

int foolishCallADD(int i, int j) {
  myScopedInterpreter t{};
  auto locals = py::dict("i"_a = i, "j"_a = j);
  py::exec(R"(
        t=i+j;
    )",
           py::globals(), locals);
  return py::cast<int>(locals["t"]);
}

PYBIND11_MODULE(callEmbed, m) {
  m.doc() = "contains the mylib modules: SimpleIO"; // optional module docstring

  m.def("foolishCallADD", &foolishCallADD,
        "A function which calculates a sum in python, passing by C++",
        py::arg("i"), py::arg("j"));
}
