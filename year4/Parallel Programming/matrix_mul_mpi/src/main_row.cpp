#include "mmul.hpp"

#include <iostream>
#include <mpi.h>

using namespace std;

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
 
    size_t rows_per_thread = rows_A / size;

    Matrix A_local(rows_per_thread, cols_A);
    Matrix A;
    Matrix B(rows_B, cols_B);
    Matrix C;

    if (rank == MASTER) {
        A = Matrix::Random(rows_A, cols_A);
        B.fill_random();
        C = Matrix(rows_A, cols_B);
    }

    // send chunks of matrix A to all processes
    MPI::COMM_WORLD.Scatter(A.raw(), A_local.size(), MPI_DOUBLE, A_local.raw(), A_local.size(), MPI_DOUBLE, MASTER);

    // distribute matrix B to all processes
    MPI::COMM_WORLD.Bcast(B.raw(), B.size(), MPI_DOUBLE, MASTER);

    double start = MPI::Wtime();

    // calculate the portion of matrix C to be computed by this process
    Matrix C_local(A_local.rows(), B.cols());
    for (size_t i = 0; i < A_local.rows(); ++i) {
        for (size_t j = 0; j < B.cols(); ++j) {
            for (size_t k = 0; k < A_local.cols(); ++k) {
                C_local(i, j) += A_local(i, k) * B(k, j);
            }
        }
    }

    // gather the portions of matrix C computed by each process to the master process
    MPI::COMM_WORLD.Gather(C_local.raw(), C_local.size(), MPI_DOUBLE, C.raw(), C_local.size(), MPI_DOUBLE, MASTER);

    double end = MPI::Wtime() - start;

    if (rank == MASTER) {
        std::cout << "Elapsed time: " << end - start << "s" << std::endl;
    }

    MPI::Finalize();
    return EXIT_SUCCESS;
}