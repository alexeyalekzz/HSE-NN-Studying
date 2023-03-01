#include "n_body.h"

#include <pthread.h>

#include <math.h>
#include <stdlib.h>

#define EPS 0.00001

static pthread_cond_t cv;
static pthread_mutex_t mutex;
static size_t cntr = 0;

typedef struct Args {
    size_t start, end;
    Particle* particles;
    size_t n_particles;
    size_t n_threads;
    double dt;
} Args;

static void* routine(void* a) {
    Args args = *(Args*)a;
    double* forces_x = (double*) malloc(sizeof(double) * (args.end - args.start));
    double* forces_y = (double*) malloc(sizeof(double) * (args.end - args.start));

    // calculates force for each particle inside chunk - [start, end)
    // also takes into account all the particles outside this chunk
    //
    // no need to synchronize this part, because threads are only reading data here
    for (size_t i = args.start; i < args.end; ++i) {
        const double Gm = G * args.particles[i].mass;
        double f_sum_x = 0, f_sum_y = 0;
        for (size_t j = 0; j < args.n_particles; ++j) {
            // calc net force projection on X and Y
            if (i != j) {
                const double dx = args.particles[j].x - args.particles[i].x;
                const double dy = args.particles[j].y - args.particles[i].y;
                const double d = sqrt(dx * dx + dy * dy);
                const double temp = Gm * args.particles[j].mass / (d * d * d);
                f_sum_x += temp * dx;
                f_sum_y += temp * dy;
            }
        }
        forces_x[i - args.start] = f_sum_x;
        forces_y[i - args.start] = f_sum_y;
    }

    {
        // execution will continue only after ALL threads gets here
        pthread_mutex_lock(&mutex);
        cntr++;
        while (cntr != args.n_threads) {
            pthread_cond_wait(&cv, &mutex);
        }
        pthread_cond_broadcast(&cv);
        pthread_mutex_unlock(&mutex);
    }

    // update parameters for each body
    //
    // after previous block we ensure that each thread done reading and now ready to update data
    // no need to acquire mutex, each thread writing in it's own chunk
    for (size_t i = args.start; i < args.end; ++i) {
        // update position
        args.particles[i].x += args.dt * args.particles[i].vx;
        args.particles[i].y += args.dt * args.particles[i].vy;
        // update velocity
        args.particles[i].vx += args.dt * forces_x[i - args.start] / args.particles[i].mass;
        args.particles[i].vy += args.dt * forces_y[i - args.start] / args.particles[i].mass;
    }

    free(forces_x);
    free(forces_y);
    pthread_exit(NULL);
}


void solve_n_body(Particle* particles, const size_t n_particles, const int n_threads, const double dt, const int last, const int check_collision, FILE* out)
{
    // csv header
    fprintf(out, "t,");
    for (size_t i = 0; i < n_particles; ++i) {
        fprintf(out, "x%ld,y%ld,", i, i);
    }
    putc('\n', out);

    // initialize pthread variables
    pthread_cond_init(&cv, NULL);
    pthread_mutex_init(&mutex, NULL);

    // divide input data for non-overlapping chunks, one chunk per thread
    pthread_t threads[n_threads];
    int chunk_size = n_particles / n_threads;

    // set up thread arguments
    Args thread_args[n_threads];
    for (size_t i = 0; i < n_threads; ++i) {
        thread_args[i].start = i * chunk_size;
        thread_args[i].end = (i + 1) * chunk_size;
        thread_args[i].dt = dt;
        thread_args[i].n_threads = n_threads;
        thread_args[i].particles = particles;
        thread_args[i].n_particles = n_particles;
    }

    // special case for last thread
    thread_args[n_threads-1].end = n_particles;

    // main loop
    for (size_t i = 0; i < last; ++i) {
        for (size_t j = 0; j < n_threads; ++j) {
            pthread_create(&threads[j], NULL, routine, &thread_args[j]);
        }
        for (size_t j = 0; j < n_threads; ++j) {
            pthread_join(threads[j], NULL);
        }
        cntr = 0;

        // check collision
        if (check_collision) {
            for (size_t p = 0; p < n_particles; ++p) {
                for (size_t q = p + 1; q < n_particles; ++q) {
                    if ((fabs(particles[p].x - particles[q].x) < EPS) && (fabs(particles[p].y - particles[q].y) < EPS)) {
                        double tvx = particles[p].vx, tvy = particles[p].vy;
                        particles[p].vx = particles[q].vx;
                        particles[p].vy = particles[q].vy;
                        particles[q].vx = tvx;
                        particles[q].vy = tvy;
                    }
                }
            }
        }

        // dump stats
        fprintf(out, "%ld,", i);
        for (size_t j = 0; j < n_particles; ++j) {
            fprintf(out, "%lf,%lf,", particles[j].x, particles[j].y);
        }
        putc('\n', out);
    }

    pthread_cond_destroy(&cv);
    pthread_mutex_destroy(&mutex);
}