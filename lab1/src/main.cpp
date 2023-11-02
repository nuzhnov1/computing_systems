#include <iostream>
#include <chrono>
#include "Argument.h"
#include "SquareMatrix.h"
#include "Time.h"

using namespace Argument;
using namespace Matrix;
using namespace Time;


int main(int argc, char const *argv[])
{
    std::size_t size = to_size_t(fetch_argument(argc, argv, 1));
    std::size_t n_threads = to_size_t(fetch_argument(argc, argv, 2));

    SquareMatrix::set_n_threads(n_threads);

    std::function<SquareMatrix(ComputingMode)> lambda = [size](ComputingMode mode) -> SquareMatrix {
        SquareMatrix::set_computing_mode(mode);

        SquareMatrix m1(size);
        SquareMatrix m2(size);

        return m1 * m2;
    };

    std::cout << "Matrix size: " << size << std::endl;
    std::cout << "Threads count: " << n_threads << std::endl;

    std::cout
        << "Sequence mode: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(elapse_time(lambda, ComputingMode::SEQUENCE).second)
        << std::endl;

    std::cout
        << "OpenMP mode: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(elapse_time(lambda, ComputingMode::OPENMP).second)
        << std::endl;

    std::cout
        << "Posix threads mode: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(elapse_time(lambda, ComputingMode::PTHREADS).second)
        << std::endl;

    std::cout
        << "TBB mode: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(elapse_time(lambda, ComputingMode::TBB).second)
        << std::endl;

    return 0;
}
