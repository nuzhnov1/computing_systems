#include "SquareMatrix.h"
#include "Random.h"

#include <pthread.h>

using namespace Matrix;


struct FillMatrixArgs {
    double* data;
    std::size_t chunk_size;
    std::size_t tid;
};

struct MultiplyMatricesArgs {
    const double* m1_data;
    const double* m2_data;
    double * result_data;
    std::size_t size;
    std::size_t chunk_size;
    std::size_t tid;
};


void SquareMatrix::fill_with_random_values_pthreads() {
    double* data = m_data;
    std::size_t size = m_size;

    std::size_t num_threads = s_n_threads;
    std::size_t data_size = size * size;
    std::size_t chunk_size;
    std::size_t rest_chunk_size;

    if (data_size > num_threads) {
        chunk_size = data_size / num_threads;
        rest_chunk_size = data_size % num_threads;
    } else {
        num_threads = data_size;
        chunk_size = 1;
        rest_chunk_size = 0;
    }

    auto* pthreads = new pthread_t[num_threads];
    auto* pthreads_args = new FillMatrixArgs[num_threads];

    for (std::size_t tid = 0; tid < num_threads; tid++) {
        pthreads_args[tid] = (FillMatrixArgs){
            .data = data,
            .chunk_size = chunk_size,
            .tid = tid
        };

        pthread_create(&pthreads[tid], nullptr, fill_with_random_values_worker, &pthreads_args[tid]);
    }

    for (std::size_t i = data_size - rest_chunk_size; i < data_size; i++) {
        data[i] = Random::rand_uniform_double();
    }

    for (std::size_t tid = 0; tid < num_threads; tid++) {
        pthread_join(pthreads[tid], nullptr);
    }

    delete[] pthreads;
    delete[] pthreads_args;
}

void* SquareMatrix::fill_with_random_values_worker(void *args) {
    FillMatrixArgs fill_matrix_args = *(FillMatrixArgs*)args;
    double* data = fill_matrix_args.data;
    std::size_t chunk_size = fill_matrix_args.chunk_size;
    std::size_t tid = fill_matrix_args.tid;

    std::size_t start = tid * chunk_size;
    std::size_t end = start + chunk_size;

    for (std::size_t i = start; i < end; i++) {
        data[i] = Random::rand_uniform_double();
    }

    return nullptr;
}

SquareMatrix SquareMatrix::multiply_pthreads(const Matrix::SquareMatrix &another) const {
    SquareMatrix result = SquareMatrix(m_size, false);

    const double* m1_data = m_data;
    const double* m2_data = another.m_data;
    double* result_data = result.m_data;
    std::size_t size = m_size;

    std::size_t num_threads = s_n_threads;
    std::size_t chunk_size;
    std::size_t rest_chunk_size;

    if (size > num_threads) {
        chunk_size = size / num_threads;
        rest_chunk_size = size % num_threads;
    } else {
        num_threads = size;
        chunk_size = 1;
        rest_chunk_size = 0;
    }

    auto* pthreads = new pthread_t[num_threads];
    auto* pthreads_args = new MultiplyMatricesArgs[num_threads];

    for (std::size_t tid = 0; tid < num_threads; tid++) {
        pthreads_args[tid] = (MultiplyMatricesArgs){
            .m1_data = m1_data,
            .m2_data = m2_data,
            .result_data = result_data,
            .size = size,
            .chunk_size = chunk_size,
            .tid = tid
        };

        pthread_create(&pthreads[tid], nullptr, multiply_worker, &pthreads_args[tid]);
    }

    for (std::size_t i = size - rest_chunk_size; i < size; i++) {
        for (std::size_t j = 0; j < size; j++) {
            result_data[i * size + j] = 0;

            for (std::size_t k = 0; k < size; k++) {
                result_data[i * size + j] += m1_data[i * size + k] * m2_data[k * size + j];
            }
        }
    }

    for (std::size_t tid = 0; tid < num_threads; tid++) {
        pthread_join(pthreads[tid], nullptr);
    }

    delete[] pthreads;
    delete[] pthreads_args;

    return result;
}

void* SquareMatrix::multiply_worker(void *args) {
    MultiplyMatricesArgs multiply_matrices_args = *(MultiplyMatricesArgs*)args;
    const double* m1_data = multiply_matrices_args.m1_data;
    const double* m2_data = multiply_matrices_args.m2_data;
    double* result_data = multiply_matrices_args.result_data;
    std::size_t size = multiply_matrices_args.size;
    std::size_t chunk_size = multiply_matrices_args.chunk_size;
    std::size_t tid = multiply_matrices_args.tid;

    std::size_t start = tid * chunk_size;
    std::size_t end = start + chunk_size;

    for (std::size_t i = start; i < end; i++) {
        for (std::size_t j = 0; j < size; j++) {
            result_data[i * size + j] = 0;

            for (std::size_t k = 0; k < size; k++) {
                result_data[i * size + j] += m1_data[i * size + k] * m2_data[k * size + j];
            }
        }
    }

    return nullptr;
}
