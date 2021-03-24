// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 11.02.2021.
//
#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#define DELTA 0.00000001
#define Kmax 3
#define Lmax 2

#include <fstream>
#include <cstring>
#include <sstream>

class CFP {
 private:
  bool** matrix;
  static inline int sizeM, sizeP, n1_all = 0;

 public:
  explicit CFP(const char* file) {
    std::ifstream in(file);

    if (in.is_open()) {
      in >> sizeM;
      in >> sizeP;

      matrix = new bool* [sizeM];
      for (int i = 0; i < sizeM; ++i) {
        matrix[i] = new bool [sizeP];
        std::memset(matrix[i], 0, sizeof(bool) * sizeP);
      }

      in.get();
      for (int i = 1; i <= sizeM; ++i) {
        std::string buf;
        getline(in, buf, '\n');
        std::stringstream ss(buf);

        int a;
        ss >> a;
        while (!ss.eof()) {
          short j;
          ss >> j;
          matrix[i - 1][j - 1] = true;
        }
      }

      in.close();
    } else {
      std::cout << "No such file!";
      exit(-1);
    }

    for (int i = 0; i < sizeM; ++i)
      for (int j = 0; j < sizeP; ++j)
        if (matrix[i][j] == 1) n1_all++;
  }
  ~CFP() {
    for (int i = 0; i < sizeM; i++)
      delete[] matrix[i];
    delete[] matrix;
  }

  struct solution {
    int* machines;
    int* parts;
    int clusters;
    friend class CFP;

    ~solution() {
      delete[] machines;
      delete[] parts;
    }
    double efficacy() const;
   private:
    const CFP& p;

    explicit solution(const CFP& _p) : p(_p), machines(new int [CFP::sizeM]), parts(new int [CFP::sizeP]), clusters(2) {
      for (int i = 0; i < CFP::sizeM; ++i)
        machines[i] = (int) rand() % clusters + 1;
      for (int i = 0; i < CFP::sizeP; ++i)
        parts[i] = (int) rand() % clusters + 1;

      // using Reduced VNS algorithm to initialize solution
      int max_iterations = 1000;
      do {
        int k = 0;

        do {
          auto s = this->shake(k);

          if (s.efficacy() > this->efficacy() + DELTA) {
            *this = s;
          } else {
            k++;
          }

        } while (k < Kmax);

      } while (--max_iterations);
    }
    solution(const solution& other) : p(other.p), clusters(other.clusters), machines(new int [CFP::sizeM]), parts(new int [CFP::sizeP]) {
      std::copy(other.machines, other.machines + CFP::sizeM, machines);
      std::copy(other.parts, other.parts + CFP::sizeP, parts);
    }
    solution& operator=(const solution& other) {
      if (&other == this) return *this;
      clusters = other.clusters;
      std::copy(other.machines, other.machines + CFP::sizeM, machines);
      std::copy(other.parts, other.parts + CFP::sizeP, parts);
      return *this;
    }
    solution(solution&& other) noexcept : p(other.p), clusters(other.clusters), machines(other.machines), parts(other.parts) {
      other.machines = nullptr;
      other.parts = nullptr;
    }
    solution& operator=(solution&& other) noexcept {
      if (&other == this) return *this;

      clusters = other.clusters;
      machines = other.machines;
      parts = other.parts;
      other.machines = nullptr;
      other.parts = nullptr;
      return *this;
    }

    solution shake(int k) const {
      return shaking_neighborhoods[k](*this);
    }

    solution find_best_neighbor(int l) const {
      return vnd_neighborhoods[l](*this);
    }

    void VND();
  };
  typedef solution (*func_pointer_t) (const solution&);
  const static func_pointer_t vnd_neighborhoods[Lmax], shaking_neighborhoods[Kmax];

  solution GVNS(int iters) const;
  static void print_solution(const solution& s) {
    std::cout << "Machines: ";
    for (int i = 0; i < sizeM; ++i)
      std::cout << s.machines[i] << ' ';
    std::cout << std::endl;
    std::cout << "Parts:    ";
    for (int i = 0; i < sizeP; ++i)
      std::cout << s.parts[i] << ' ';
    std::cout << std::endl
              << "Number of clusters: " << s.clusters << std::endl
              << "Efficacy: " << s.efficacy() << std::endl;
  }
};

#endif  // _PROBLEM_H_
