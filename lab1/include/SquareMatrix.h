#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

#include <cstdlib>
#include "ComputingMode.h"


namespace Matrix {
    class SquareMatrix {
    private:
        double* m_data;
        std::size_t m_size;

        static ComputingMode s_mode;
        static std::size_t s_n_threads;

    private:
        void fill_with_random_values_sequence();
        void fill_with_random_values_omp();
        void fill_with_random_values_pthreads();
        void fill_with_random_values_tbb();

        static void* fill_with_random_values_worker(void* args);

        [[nodiscard]] SquareMatrix multiply_sequence(const SquareMatrix& another) const;
        [[nodiscard]] SquareMatrix multiply_omp(const SquareMatrix& another) const;
        [[nodiscard]] SquareMatrix multiply_pthreads(const SquareMatrix& another) const;
        [[nodiscard]] SquareMatrix multiply_tbb(const SquareMatrix& another) const;

        static void* multiply_worker(void *args);

    public:
        explicit SquareMatrix(std::size_t size, bool fill_random_values = true);
        SquareMatrix(const SquareMatrix& another) = delete;
        SquareMatrix(SquareMatrix&& another) noexcept;
        ~SquareMatrix();

        SquareMatrix& operator=(SquareMatrix&& another) noexcept;
        SquareMatrix operator*(const SquareMatrix& another) const;

        static void set_computing_mode(ComputingMode value) { s_mode = value; }
        static void set_n_threads(std::size_t value) { s_n_threads = value; }
    };
}


#endif //SQUARE_MATRIX_H
