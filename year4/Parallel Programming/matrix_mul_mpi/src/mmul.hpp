#ifndef _MMUL_HPP_
#define _MMUL_HPP_

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <vector>

#define MASTER 0 // rank of the master process


struct Matrix {
    inline Matrix()
        : N(0)
        , M(0)
        , data({})
    {}

    inline Matrix(size_t N, size_t M)
        : N(N)
        , M(M)
        , data(N * M, 0.0)
    {}

    static inline Matrix Zeros(size_t N, size_t M) {
        return Matrix(N, M);
    }

    static inline Matrix Random(size_t N, size_t M) {
        Matrix ret(N, M);
        ret.fill_random();
        return ret;
    }

    inline double* raw() {
        return data.data();
    }

    inline const double* raw() const {
        return data.data();
    }

    inline size_t size() const {
        return N * M;
    }

    inline size_t rows() const {
        return N;
    }

    inline size_t cols() const {
        return M;
    }

    inline double& operator()(size_t i, size_t j) {
        return data[i * M + j];
    }

    inline double operator()(size_t i, size_t j) const {
        return data[i * M + j];
    }

    void fill_random() {
        for (size_t i = 0; i < N * M; ++i) {
            data[i] = rand() / (double)RAND_MAX;
        }
    }

    size_t N, M;
    std::vector<double> data;
};

inline std::ostream& operator<<(std::ostream& out, const Matrix& mat) {
    for (size_t i = 0; i < mat.rows(); ++i) {
        for (size_t j = 0; j < mat.cols(); ++j) {
            out << mat(i, j) << ' ';
        }
        out << '\n';
    }
    return out;
}

#endif // _MMUL_HPP_