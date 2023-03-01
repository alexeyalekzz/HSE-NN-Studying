#ifndef _N_BODY_H_
#define _N_BODY_H_

#include <stddef.h>
#include <stdio.h>

#define G (6.674e-11)

typedef struct Particle {
    double mass;
    double x, y;
    double vx, vy;
} Particle;

void solve_n_body(Particle* particles,
                  size_t n_particles,
                  const int n_threads,
                  const double dt,
                  const int last,
                  const int check_collision,
                  FILE* out);


#endif
