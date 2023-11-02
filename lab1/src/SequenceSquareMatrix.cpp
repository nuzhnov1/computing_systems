#include "SquareMatrix.h"
#include "Random.h"

using namespace Matrix;


void SquareMatrix::fill_with_random_values_sequence() {
    double* data = m_data;
    std::size_t size = m_size;
    std::size_t items_count = size * size;

    for (std::size_t i = 0; i < items_count; i++) {
        data[i] = Random::rand_uniform_double();
    }
}

SquareMatrix SquareMatrix::multiply_sequence(const Matrix::SquareMatrix &another) const {
    SquareMatrix result = SquareMatrix(m_size, false);

    const double* m1_data = m_data;
    const double* m2_data = another.m_data;
    double* result_data = result.m_data;
    std::size_t size = m_size;

    for (std::size_t i = 0; i < size; i++) {
        for (std::size_t j = 0; j < size; j++) {
            result_data[i * size + j] = 0;

            for (std::size_t k = 0; k < size; k++) {
                result_data[i * size + j] += m1_data[i * size + k] * m2_data[k * size + j];
            }
        }
    }

    return result;
}
