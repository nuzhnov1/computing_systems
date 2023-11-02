#include <iostream>
#include <cassert>
#include <cstring>
#include <mpi.h>
#include "MpiSquareMatrix.h"
#include "Random.h"

using namespace Matrix;


MpiSquareMatrix::MpiSquareMatrix(std::size_t size, bool fill_random_values) {
    int comm_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    if (comm_rank == 0) {
        m_size = size;
        m_data = new double[size * size];
    } else {
        m_size = 0;
        m_data = nullptr;
    }

    if (fill_random_values) {
        fill_with_random_values_mpi();
    }
}

MpiSquareMatrix::MpiSquareMatrix(MpiSquareMatrix&& another) noexcept :
    m_data(another.m_data),
    m_size(another.m_size) {

    another.m_size = 0;
    another.m_data = nullptr;
}

MpiSquareMatrix::~MpiSquareMatrix() { delete[] m_data; }

MpiSquareMatrix& MpiSquareMatrix::operator=(MpiSquareMatrix&& another) noexcept {
    m_data = another.m_data;
    m_size = another.m_size;

    another.m_size = 0;
    another.m_data = nullptr;

    return *this;
}

MpiSquareMatrix MpiSquareMatrix::operator*(const Matrix::MpiSquareMatrix &another) const {
    int comm_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    if (comm_rank == 0 && m_size != another.m_size) {
        std::cerr << "matrix sizes are not equals" << std::endl;
        assert(false);
    }

    return multiply_mpi(another);
}

void MpiSquareMatrix::fill_with_random_values_mpi() {
    int comm_size;
    int comm_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    std::size_t data_size = m_size * m_size;

    MPI_Bcast(&data_size, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    std::size_t chunk_data_size = data_size / comm_size;
    std::size_t rest_chunk_data_size = data_size % comm_size;

    if (chunk_data_size > 0) {
        auto* chunk = new double[chunk_data_size];

        for (std::size_t i = 0; i < chunk_data_size; i++) {
            chunk[i] = Random::rand_uniform_double();
        }

        MPI_Gather(
                chunk,
                static_cast<int>(chunk_data_size),
                MPI_DOUBLE,
                m_data,
                static_cast<int>(chunk_data_size),
                MPI_DOUBLE,
                0,
                MPI_COMM_WORLD
        );

        delete[] chunk;
    }

    if (comm_rank == 0) {
        for (std::size_t i = data_size - rest_chunk_data_size; i < data_size; i++) {
            m_data[i] = Random::rand_uniform_double();
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
}

MpiSquareMatrix MpiSquareMatrix::multiply_mpi(const Matrix::MpiSquareMatrix &another) const {
    MpiSquareMatrix result(m_size, false);
    double* result_data = result.m_data;

    int comm_size;
    int comm_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    std::size_t size = m_size;
    MPI_Bcast(&size, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    std::size_t chunk_size = size / comm_size;
    std::size_t rest_chunk_size = size % comm_size;

    if (chunk_size > 0) {
        std::size_t data_size = size * size;

        auto* another_data = new double[data_size];
        if (comm_rank == 0) std::memcpy(another_data, m_data, data_size * sizeof(double));
        MPI_Bcast(another_data, static_cast<int>(data_size), MPI_DOUBLE, 0, MPI_COMM_WORLD);

        auto* chunk = new double[chunk_size * size];
        MPI_Scatter(
                m_data,
                static_cast<int>(size * chunk_size),
                MPI_DOUBLE,
                chunk,
                static_cast<int>(size * chunk_size),
                MPI_DOUBLE,
                0,
                MPI_COMM_WORLD
        );

        auto* result_chunk = new double[chunk_size * size];

        for (std::size_t i = 0; i < chunk_size; i++) {
            for (std::size_t j = 0; j < size; j++) {
                result_chunk[i * size + j] = 0;

                for (std::size_t k = 0; k < size; k++) {
                    result_chunk[i * size + j] += chunk[i * size + k] * another_data[k * size + j];
                }
            }
        }

        MPI_Gather(
                result_chunk,
                static_cast<int>(chunk_size * size),
                MPI_DOUBLE,
                result_data,
                static_cast<int>(chunk_size * size),
                MPI_DOUBLE,
                0,
                MPI_COMM_WORLD
        );

        delete[] result_chunk;
        delete[] chunk;
        delete[] another_data;
    }

    if (comm_rank == 0) {
        double* another_data = another.m_data;

        for (std::size_t i = size - rest_chunk_size; i < size; i++) {
            for (std::size_t j = 0; j < size; j++) {
                result_data[i * size + j] = 0;

                for (std::size_t k = 0; k < size; k++) {
                    result_data[i * size + j] += m_data[i * size + k] * another_data[k * size + j];
                }
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    return result;
}
