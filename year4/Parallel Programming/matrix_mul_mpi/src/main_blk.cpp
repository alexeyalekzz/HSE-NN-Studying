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

    if (!(rows_A == cols_A && rows_B == cols_B)) {
        if (rank == MASTER) {
            std::cout << "Works only for square matrixes" << std::endl;
        }
        MPI::Finalize();
        return EXIT_FAILURE;
    }
 
    const size_t N = rows_A;
    size_t block_number = size;
    size_t block_side = rows_A / size;
    size_t block_len = N * N / block_number;

    if (block_side == 0) {
        if (rank == MASTER) {
            std::cout << "Number of threads should be less than matrix size" << std::endl;
        }
        MPI::Finalize();
        return EXIT_FAILURE;
    }

    Matrix B(rows_B, cols_B);
    std::vector<double> A_local(block_len);
    Matrix C;

    if (rank == MASTER) {
        C = Matrix(rows_A, cols_B);

        // Matrix A exists only in master
        Matrix A = Matrix::Random(rows_A, cols_A);

        B.fill_random();

        if (A.size() < 100 && B.size() < 100) {
            std::cout << "Matrix A:\n" << A << std::endl;
            std::cout << "Matrix B:\n" << B << std::endl;
        }

        std::vector<double> A_temp(block_len);
        for (size_t t_number = 0; t_number < size; ++t_number) {
            size_t q = 0;
            size_t k = 0;
            for (size_t i = 0; i < N; ++i) {
                if (i % block_side == 0 && i != 0) {
                    k += block_side;
                }
                for (size_t j = k; j < k + block_side; ++j) {
                    A_temp[q++] = A(i, (j + t_number * block_side) % N);
                }
            }

            if (t_number == MASTER) {
                A_local = A_temp;
            } else {
                MPI::COMM_WORLD.Send(A_temp.data(), A_temp.size(), MPI::DOUBLE, t_number, t_number);
            }
        }
    }
    
    if (rank != MASTER) {
        MPI::COMM_WORLD.Recv(A_local.data(), N * N, MPI::DOUBLE, MASTER, rank);
    }

    // distribute matrix B to all processes
    MPI::COMM_WORLD.Bcast(B.raw(), B.size(), MPI::DOUBLE, MASTER);

    double start = MPI::Wtime();

    Matrix C_local = Matrix::Zeros(N, N);
    size_t q = rank * block_side;
    for (size_t i = 0; i < N; ++i) {
        if (i % block_side == 0 && i != 0) {
            q = (q + block_side) % N;
        }
        for (size_t j = 0; j < N; ++j) {
            for (size_t k = 0; k < block_side; ++k) {
                C_local(i, j) += A_local[k + block_side * i] * B((k + q), j);
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
