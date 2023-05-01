#include "mmul.hpp"

#include <iostream>
#include <mpi.h>


int main(int argc, char **argv) {
    size_t rows_A, cols_A;
    size_t rows_B, cols_B;

    MPI::Init(argc, argv);

    size_t rank = MPI::COMM_WORLD.Get_rank();
    size_t size = MPI::COMM_WORLD.Get_size();

    try {
        rows_A = std::stoul(argv[1]);
        cols_A = std::stoul(argv[2]);
        rows_B = std::stoul(argv[3]);
        cols_B = std::stoul(argv[4]);
    } catch (std::exception e) {
        if (rank == MASTER) {
            std::cout << "Unexpected error occured while parsing argument list: " << e.what() << std::endl;
        }
        MPI::Finalize();
        return EXIT_FAILURE;
    }
 
    size_t cols_per_thread = cols_A / size;

    Matrix A(rows_A, cols_A);
    Matrix B;
    Matrix B_local(cols_per_thread, cols_B);
    Matrix C;

    if (rank == MASTER) {
        A.fill_random();
        B = Matrix::Random(rows_B, cols_B);
        C = Matrix(rows_A, cols_B);

        if (A.size() < 100 && B.size() < 100) {
            std::cout << "Matrix A:\n" << A << std::endl;
            std::cout << "Matrix B:\n" << B << std::endl;
        }
    }

    // send chunks of matrix B to all processes
    MPI::COMM_WORLD.Scatter(B.raw(), B_local.size(), MPI::DOUBLE, B_local.raw(), B_local.size(), MPI::DOUBLE, MASTER);

    // distribute matrix A to all processes
    MPI::COMM_WORLD.Bcast(A.raw(), A.size(), MPI::DOUBLE, MASTER);

    double start = MPI::Wtime();

    // calculate the portion of matrix C to be computed by this process
    Matrix C_local(rows_A, cols_B);
    for (size_t j = 0; j < cols_per_thread; ++j) {
        for (size_t i = 0; i < A.rows(); ++i) {
            for (size_t k = 0; k < B_local.cols(); ++k) {
                C_local(i, k) += A(i, j + rank * cols_per_thread) * B_local(j, k);
            }
        }
    }

    // gather the subsums of matrix C computed by each process to the master process
    MPI::COMM_WORLD.Reduce(C_local.raw(), C.raw(), C_local.size(), MPI::DOUBLE, MPI::SUM, MASTER);

    double end = MPI::Wtime();

    if (rank == MASTER) {
        std::cout << "Elapsed time: " << end - start << "s" << std::endl;
    }

    if (rank == MASTER && C.size() < 100) {
        std::cout << "Matrix C:\n" << C << std::endl;
    }

    MPI::Finalize();
    return EXIT_SUCCESS;
}