// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 15.03.2021.
//

#ifndef GENETICTSP_H_
#define GENETICTSP_H_

#include <array>
#include <fstream>
#include <cmath>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <random>
#include <vector>

#define SIZE 1000
#define POP_SIZE 20
#define MUT_PROBABILITY 10
#define PARENTS_N 5
#define EPS 0.000000001

constexpr int fact(int n) {
  return n <= 1 ? 1 : n * fact(n - 1);
}
constexpr int combination(int n, int k) {
  return fact(n) / (fact(k) * fact(n - k));
}
constexpr int CHILDREN_N = combination(PARENTS_N, 2) * 2;


bool is_prime(unsigned long a) {
  unsigned long i;
  if (a == 2)
    return true;
  if (a == 0 || a == 1 || a % 2 == 0)
    return false;
  for(i = 3; i*i <= a && a % i; i += 2) {}
  return i*i > a;
}


using chromosome = std::vector<int>;
template<size_t size>
using group = std::array<chromosome, size>;


class Genetic {
  double** data;

 public:
  explicit Genetic(const char* file) {
    data = new double* [SIZE];
    for (int i = 0; i < SIZE; ++i)
      data[i] = new double [SIZE];

    std::ifstream in(file);
    auto text = new std::string [SIZE];

    if (in.is_open()) {
      for (int i = 0; i < SIZE; ++i)
        std::getline(in, text[i]);

      for (int i = 0; i < SIZE; ++i) {
        std::stringstream ss(text[i]);
        int p;
        double x1, y1;
        ss >> p >> x1 >> y1;
        data[i][i] = 0;

        for (int j = i + 1; j < SIZE; ++j) {
          std::stringstream ss1(text[j]);
          double x2, y2;
          ss1 >> p >> x2 >> y2;

          data[i][j] = std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
          data[j][i] = data[i][j];
        }
      }
    } else exit(-1);
    in.close();
  }
  ~Genetic() {
    for (int i{0}; i < SIZE; ++i)
      delete[] data[i];
    delete[] data;
  }

  [[nodiscard]] std::string print_solution(const chromosome& c) const {
    std::string res;
    for (auto& n : c)
      (res += std::to_string(n)) += ' ';
    res += '\n';
    res += std::to_string(cost(c));
    return res += '\n';
  }

  [[nodiscard]] chromosome start_genetics(int max_iters) const;

 private:
  /* Chromosome */
  [[nodiscard]] double cost(const chromosome& s) const;
  void eval(chromosome& s) const;
  static void init(chromosome& s);
  static void mutate(chromosome& s);
  static void cycle_crossover(const chromosome& p1, const chromosome& p2, chromosome& c1, chromosome& c2);
  /*************/


  /* Group */
  [[nodiscard]] group<PARENTS_N> tournament_selection(const group<POP_SIZE>& g) const;
  [[nodiscard]] chromosome find_best(const group<POP_SIZE>& g) const;
  [[nodiscard]] group<CHILDREN_N> reproduction(const group<POP_SIZE>& g) const;
  group<POP_SIZE> build_next_steady_state(group<POP_SIZE>& p, group<CHILDREN_N>& c) const;
  static void initialize(group<POP_SIZE>& g);
  template<size_t size> static void modification(group<size>& g);
  template<size_t size> void evaluation(group<size>& g) const;
  /*************/
};



#endif  // GENETICTSP_H_
