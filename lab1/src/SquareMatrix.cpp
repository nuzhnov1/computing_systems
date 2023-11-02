#include <iostream>
#include <cassert>
#include "SquareMatrix.h"

using namespace Matrix;


ComputingMode SquareMatrix::s_mode = ComputingMode::SEQUENCE;
std::size_t SquareMatrix::s_n_threads = 4;


SquareMatrix::SquareMatrix(std::size_t size, bool fill_random_values) :
        m_size(size),
        m_data(new double[size * size]) {

    if (fill_random_values) {
        switch (s_mode) {
            case ComputingMode::SEQUENCE: fill_with_random_values_sequence();
            case ComputingMode::OPENMP: fill_with_random_values_omp();
            case ComputingMode::PTHREADS: fill_with_random_values_pthreads();
            case ComputingMode::TBB: fill_with_random_values_tbb();
        }
    }
}

SquareMatrix::SquareMatrix(SquareMatrix&& another) noexcept :
    m_data(another.m_data),
    m_size(another.m_size) {

    another.m_size = 0;
    another.m_data = nullptr;
}

SquareMatrix::~SquareMatrix() { delete[] m_data; }

SquareMatrix& SquareMatrix::operator=(SquareMatrix&& another) noexcept {
    m_data = another.m_data;
    m_size = another.m_size;

    another.m_size = 0;
    another.m_data = nullptr;

    return *this;
}

SquareMatrix SquareMatrix::operator*(const Matrix::SquareMatrix &another) const {
    if (m_size != another.m_size) {
        std::cerr << "matrix sizes are not equals" << std::endl;
        assert(false);
    }

    switch (s_mode) {
        case ComputingMode::OPENMP: return multiply_omp(another);
        case ComputingMode::PTHREADS: return multiply_pthreads(another);
        case ComputingMode::TBB: return multiply_tbb(another);
        default: return multiply_sequence(another);
    }
}
