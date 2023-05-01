function usage {
    echo "Wrong number of parameters"
    echo "Usage: ./build_n_run.sh <mode> <number_of_threads> <rows_A> <cols_A> <rows_B> <cols_B>"
    echo "    mode    - 'row' / 'col' / 'block'"
    exit -1
}

OPT=""

if [ "$#" -lt 6 ]; then
    usage
elif [[ "$1" == "row" ]]; then
    if [ $(("$3" % "$2")) -ne 0 ]; then
        echo "Number of rows in the matrices must be divisible by the number of processes."
        exit -1
    fi

    mpicxx $OPT src/main_row.cpp -o mmul_row && { mpiexec --use-hwthread-cpus -n "$2" mmul_row "${@:3}"; }
elif [[ "$1" == "col" ]]; then
    if [ $(("$4" % "$2")) -ne 0 ]; then
        echo "Number of cols in the matrices must be divisible by the number of processes."
        exit -1
    fi

    mpicxx $OPT src/main_col.cpp -o mmul_col && { mpiexec --use-hwthread-cpus -n "$2" mmul_col "${@:3}"; }
elif [[ "$1" == "block" ]]; then

    mpicxx $OPT src/main_blk.cpp -o mmul_blk && { mpiexec --use-hwthread-cpus -n "$2" mmul_blk "${@:3}"; }
fi
