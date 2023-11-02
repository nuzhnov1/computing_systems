#include <iostream>
#include <chrono>
#include <mpi.h>
#include "Argument.h"
#include "MpiSquareMatrix.h"
#include "Time.h"

using namespace Argument;
using namespace Matrix;
using namespace Time;


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int comm_size;
    int comm_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    std::size_t size = to_size_t(fetch_argument(argc, argv, 1));

    std::function<MpiSquareMatrix()> lambda = [size]() -> MpiSquareMatrix {
        MpiSquareMatrix m1(size);
        MpiSquareMatrix m2(size);

        return m1 * m2;
    };

    if (comm_rank == 0) {
        std::cout << "Matrix size: " << size << std::endl;
        std::cout << "Nodes count: " << comm_size << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(elapse_time(lambda).second);

    if (comm_rank == 0) {
        std::cout << "Elapsed time: " << elapsed_time << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}
