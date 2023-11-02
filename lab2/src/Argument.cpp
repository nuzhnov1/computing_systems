#include <iostream>
#include <cassert>
#include <sstream>
#include "mpi.h"
#include "Argument.h"


char const* Argument::fetch_argument(int argc, char const* const argv[], size_t argn) {
    if (argc < argn + 1) {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if (rank == 0) {
            std::cerr << argn << " argument is absent" << std::endl;
        }

        assert(false);
    }

    return argv[argn];
}

std::size_t Argument::to_size_t(const char* arg) {
    std::stringstream stream(arg);
    std::size_t result;
    stream >> result;

    return result;
}
