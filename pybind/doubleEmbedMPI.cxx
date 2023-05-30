#include <iostream>
#include <mpi.h>
#include <pybind11/embed.h> // everything needed for embedding

namespace py = pybind11;
using namespace py::literals;
using std::cout;
#define CC std::cout << "Line: " << __LINE__ << "\n";

void func(const int j) {

  auto locals = py::dict("j"_a = 6);
  locals["j"] = j;
  py::exec(R"(
        print(f"pyproc {j}")
    )",
           py::globals(), locals);
}

// must be imporved a lot, but RAII ^^
struct mpiGuard final {
  mpiGuard() { MPI_Init(NULL, NULL); }
  ~mpiGuard() {
    int finalized;
    MPI_Finalized(&finalized);
    if (!finalized) {
      MPI_Finalize();
    }
  }
};

int main(int, char **) {
  mpiGuard guard{};

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  try {
    int j;
    MPI_Comm_rank(MPI_COMM_WORLD, &j);
    py::scoped_interpreter t{};
    std::cout << "c++proc" << j << "/" << world_size << "\n";

    func(j);

  } catch (pybind11::error_already_set &x) {
    cout << "pybind11::error_already_set\n";
  }

  return 0;
}
