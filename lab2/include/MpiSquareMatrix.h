#ifndef SQUARE_MATRIX
#define SQUARE_MATRIX

#include <cstdlib>


namespace Matrix {
    class MpiSquareMatrix {
    private:
        double* m_data;
        std::size_t m_size;

        void fill_with_random_values_mpi();
        [[nodiscard]] MpiSquareMatrix multiply_mpi(const MpiSquareMatrix& another) const;

    public:
        explicit MpiSquareMatrix(std::size_t size, bool fill_random_values = true);
        MpiSquareMatrix(const MpiSquareMatrix& another) = delete;
        MpiSquareMatrix(MpiSquareMatrix&& another) noexcept;
        ~MpiSquareMatrix();

        MpiSquareMatrix& operator=(MpiSquareMatrix&& another) noexcept;
        MpiSquareMatrix operator*(const MpiSquareMatrix& another) const;
    };
}


#endif //SQUARE_MATRIX
