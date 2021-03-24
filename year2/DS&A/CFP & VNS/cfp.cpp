// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 11.02.2021.
//
#include <iostream>
#include "problem.h"

#define DELTA 0.00000001
#define Kmax 3
#define Lmax 2

const CFP::func_pointer_t CFP::vnd_neighborhoods[Lmax] {
    /* swap rows */
  [] (const solution& s) {
    solution best_solution (s);
    auto best_efficacy = s.efficacy();
    solution s1 (s);

    for (int c = 1; c <= s.clusters; ++c) {
      for (int i = 0; i < sizeM; ++i) {
        if (s.machines[i] == c) {
          for (int j = 0; j < sizeM; ++j) {
            if (s.machines[j] != c) {
              int temp = s1.machines[i];
              s1.machines[i] = s1.machines[j];
              s1.machines[j] = temp;

              if (auto eff = s1.efficacy(); eff > best_efficacy + DELTA) {
                std::copy(s1.machines, s1.machines + CFP::sizeM, best_solution.machines);
                best_efficacy = eff;
              }
              s1.machines[j] = s1.machines[i];
              s1.machines[i] = temp;
            }
          }
        }
      }
    }

    return best_solution;
  },

    /* swap columns */
  [] (const solution& s) {
    solution best_solution (s);
    auto best_efficacy = s.efficacy();
    solution s1 (s);

    for (int c = 1; c <= s.clusters; ++c) {
      for (int i = 0; i < sizeP; ++i) {
        if (s.parts[i] == c) {
          for (int j = 0; j < sizeP; ++j) {
            if (s.parts[j] != c) {
              int temp = s1.parts[i];
              s1.parts[i] = s1.parts[j];
              s1.parts[j] = temp;

              if (auto eff = s1.efficacy(); eff > best_efficacy + DELTA) {
                std::copy(s1.parts, s1.parts + CFP::sizeP, best_solution.parts);
                best_efficacy = eff;
              }
              s1.parts[j] = s1.parts[i];
              s1.parts[i] = temp;
            }
          }
        }
      }
    }

    return best_solution;
  },
};

const CFP::func_pointer_t CFP::shaking_neighborhoods[Kmax] {
    /* split clusters */
  [] (const solution& s) {
    solution x (s);
    int mi, pj;
    x.clusters++;

    int k, l;

    do {
      k = 0; l = 0;
      do {
        mi = (int) rand() % sizeM;
        pj = (int) rand() % sizeP;
      } while (x.machines[mi] != x.parts[pj]);

      for (int i = 0; i < CFP::sizeM; ++i)
        if (x.machines[i] == x.machines[mi])
          k++;
      for (int i = 0; i < CFP::sizeP; ++i)
        if (x.parts[i] == x.parts[pj])
          l++;

    } while (k == 1 || l == 1);

    x.machines[mi] = x.clusters;
    x.parts[pj] = x.clusters;

    return x;
  },

    /* merge clusters */
  [] (const solution& s) {
    if (s.clusters <= 2) return s;

    solution x (s);
    int cluster1, cluster2;

    do {
      cluster1 = (int) rand() % x.clusters + 1;
      cluster2 = (int) rand() % x.clusters + 1;
    } while (cluster1 == cluster2);

    for (int i = 0; i < CFP::sizeM; ++i) {
      if (x.machines[i] == cluster2) x.machines[i] = cluster1;
    }

    for (int i = 0; i < CFP::sizeM; ++i) {
      if (x.machines[i] > cluster2) x.machines[i]--;
    }

    for (int i = 0; i < CFP::sizeP; ++i) {
      if (x.parts[i] == cluster2) x.parts[i] = cluster1;
    }

    for (int i = 0; i < CFP::sizeP; ++i) {
      if (x.parts[i] > cluster2) x.parts[i]--;
    }

    x.clusters--;
    return x;
  },

    /* split + merge */
  [] (const solution& s) {
    return s.shake(0).shake(1);
  },
};

double CFP::solution::efficacy() const {
  double n0_in = 0, n1_in = 0;

  for (int i = 0; i < CFP::sizeM; ++i)
    for (int j = 0; j < CFP::sizeP; ++j)
      if (p.matrix[i][j] && machines[i] == parts[j])
        n1_in++;
      else if (machines[i] == parts[j])
        n0_in++;

  return n1_in / (CFP::n1_all + n0_in);
}

void CFP::solution::VND() {
  int no_improve_iterations = 0;
  int l = 0;
  do {
    solution best_neighbor = this->find_best_neighbor(l % Lmax);

    if (best_neighbor.efficacy() > this->efficacy() + DELTA) {
      *this = best_neighbor;
      no_improve_iterations = 0;
    } else {
      no_improve_iterations++;
    }
    l++;

  } while (no_improve_iterations < 10);
}

CFP::solution CFP::GVNS(int max_iterations) const {

  solution s(*this); // initial solution
  do {
    int k = 0;

    do {
      solution s1 = s.shake(k);
      s1.VND();

      if (auto ef = s1.efficacy(); ef > s.efficacy() + DELTA) {
        s = s1;
        k = 0;
        std::cout << ef << ' ' << max_iterations << '\n';
      } else {
        k++;
      }

    } while (k < Kmax);
  } while (--max_iterations);

  return s;
}


int main() {
  std::string filename;
  std::cin >> filename;

  CFP p(filename.c_str());

  auto s = p.GVNS(100);

  CFP::print_solution(s);
}