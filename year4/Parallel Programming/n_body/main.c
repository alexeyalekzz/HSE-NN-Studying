#include "n_body.h"

#include <stdlib.h>
#include <string.h>


typedef struct Args {
    int n_threads;
    double time_delta;
    int t_last;
    FILE* outfile;
    int check_collision;
    Particle* particles;
    size_t n_particles;
} Args;


static void parse_file(Args* args, const char* filename) {
    FILE* fp;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Could not open file %s\n", filename);
        exit(-1);
    }

    int _ = fscanf(fp, "%ld", &args->n_particles);

    args->particles = (Particle*) malloc(sizeof(Particle) * args->n_particles);

    size_t i = 0;
    double mass, x, y, vx, vy;
    while (fscanf(fp, "%lf\n%lf %lf\n%lf %lf", &mass, &x, &y, &vx, &vy) != EOF) {
        args->particles[i].mass = mass;
        args->particles[i].x = x;
        args->particles[i].y = y;
        args->particles[i].vx = vx;
        args->particles[i].vy = vy;
        ++i;
    }

    fclose(fp);
}

static Args handle_args(int argc, char** argv) {
    Args args = { 4, 0.1, 100, stdout, 1, 0, 0 };

    for (int i = 1; i < argc; ++i) {
        if (strcmp("-j", argv[i]) == 0) {
            args.n_threads = atoi(argv[i + 1]);
            ++i;
        } else if (strcmp("-dt", argv[i]) == 0) {
            args.time_delta = atof(argv[i + 1]);
            ++i;
        } else if (strcmp("-i", argv[i]) == 0) {
            args.t_last = atoi(argv[i+1]);
            ++i;
        } else if (strcmp("--no-collision", argv[i]) == 0) {
            args.check_collision = 0;
        } else if (strcmp("-o", argv[i]) == 0) {
            FILE* fp = fopen(argv[i+1], "w");
            if (fp == NULL) {
                printf("Could not open file %s\n", argv[i+1]);
                exit(-1);
            }
            args.outfile = fp;
            ++i;
        } else {
            parse_file(&args, argv[i]);
        }
    }

    return args;
}

static void usage() {
    printf(
        "Usage: ./n_body [options] input_filename\n\n"
        "Options:\n"
        "\t-j  n_threads\t\tnumber of threads to run (default = 4)\n"
        "\t-dt time_delta\t\tthe amount by which time changes on each computation step (default = 0.1)\n"
        "\t-i  iterations\t\tnumber of iterations to run (default = 100)\n"
        "\t--no-collision\t\tdisables collision between bodies\n"
        "\t-o  filename\t\tspecify output file (default = stdout)\n"
        "\n"
    );
    exit(-1);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        usage();
    }

    Args args = handle_args(argc, argv);

    printf(
        "Run parameters:\n"
        "    Number of threads: %d\n"
        "    Number of bodies:  %ld\n"
        "    Iterations:        %d\n"
        "    Time delta:        %lf\n"
        "    Collision:         %s\n\n",
        args.n_threads, args.n_particles, args.t_last, args.time_delta, (args.check_collision ? "Enabled" : "Disabled")
    );

    solve_n_body(
        args.particles,
        args.n_particles,
        args.n_threads,
        args.time_delta,
        args.t_last,
        args.check_collision,
        args.outfile
    );

    free(args.particles);
    fclose(args.outfile);
    return 0;
}
