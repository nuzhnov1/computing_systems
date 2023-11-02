#include <tbb/parallel_for.h>
#include "SquareMatrix.h"
#include "Random.h"

using namespace Matrix;


void SquareMatrix::fill_with_random_values_tbb() {
    double* data = m_data;
    std::size_t size = m_size;
    std::size_t items_count = size * size;
    std::size_t n_threads = s_n_threads;
    std::size_t chunk_size;

    if (items_count > n_threads) {
        chunk_size = items_count / n_threads;
    } else {
        chunk_size = 1;
    }

    tbb::parallel_for(
        tbb::blocked_range<std::size_t>(0, items_count, chunk_size),
        [data] (tbb::blocked_range<std::size_t> range) noexcept -> void {
            for (std::size_t i = range.begin(); i < range.end(); i++) {
                data[i] = Random::rand_uniform_double();
            }
        }
    );
}

SquareMatrix SquareMatrix::multiply_tbb(const Matrix::SquareMatrix &another) const {
    SquareMatrix result = SquareMatrix(m_size, false);

    const double* m1_data = m_data;
    const double* m2_data = another.m_data;
    double* result_data = result.m_data;
    std::size_t size = m_size;
    std::size_t n_threads = s_n_threads;
    std::size_t chunk_size;

    if (size > n_threads) {
        chunk_size = size / n_threads;
    } else {
        chunk_size = 1;
    }

    tbb::parallel_for(
        tbb::blocked_range<std::size_t>(0, size, chunk_size),
        [m1_data, m2_data, result_data, size] (tbb::blocked_range<std::size_t> range) noexcept -> void {
            for (std::size_t i = range.begin(); i < range.end(); i++) {
                for (std::size_t j = 0; j < size; j++) {
                    result_data[i * size + j] = 0;

                    for (std::size_t k = 0; k < size; k++) {
                        result_data[i * size + j] += m1_data[i * size + k] * m2_data[k * size + j];
                    }
                }
            }
        }
    );

    return result;
}
