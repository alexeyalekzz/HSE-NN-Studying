#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <cmath>
#include <sstream>

#define size 1000

using namespace std;

double** parse_file(const char* filename) {
  ifstream file(filename);
  auto arr = new double* [size];
  for (int i = 0; i < size; i++)
    arr[i] = new double [size];

  int trash;
  file >> trash;

  int data[size][3];

  if (file.is_open()) std::cout << "File opened!\n";

  int i = 0;
  while (i < 1000) {
    file >> data[i][0] >> data[i][1] >> data[i][2];
    i++;
  }

  for (i = 0; i < size; i++) {
    auto x1 = data[i][1];
    auto y1 = data[i][2];
    arr[i][i] = 0;
    for (int j = i + 1; j < size; j++) {
      auto x2 = data[j][1];
      auto y2 = data[j][2];
      arr[i][j] = sqrt( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) );
      arr[j][i] = arr[i][j];
    }
  }

  return arr;
}

namespace tsp {


static double** Graph;

struct solution {
  vector<int> path;
  double cost;

  solution():
      cost(0),
      path(size)
    {}

  explicit solution(vector<int> p, double c):
      cost(c),
      path(std::move(p))
    {}

  solution(const solution&) = default;

  void initialize() {
    for (int i = 0; i < size; i++)
      path[i] = size - 1 - i;

    for (int i = 1; i < size; i++)
      cost += Graph[path[i-1]][path[i]];
    cost += Graph[0][size - 1];
  }

  solution perturbation() {
    auto s (*this);
    s = s.two_opt();
    s = s.two_opt();

    return s;
  }
  solution perturbation_rand() {
    auto s (*this);
    s = s.two_opt_rand();
    s = s.two_opt_rand();

    return s;
  }

  solution local_search() {
    double op_cost = cost;
    int op_first;
    int op_first_next;
    int op_second;
    int op_second_next;
    long n = size * size;
    while (n--) {
      int first       = (int) rand() % size;
      int first_next  = first + 1 == size ? 0 : first + 1;
      int second      = ((int) rand() % (size - 3) + 2 + first) % size;
      int second_next = second + 1 == size ? 0 : second + 1;

      double new_cost;
        if (first != second) {
          new_cost = cost - Graph[path[first]][path[first_next]] - Graph[path[second]][path[second_next]]
              + Graph[path[first]][path[second]] + Graph[path[first_next]][path[second_next]];

          if (new_cost < op_cost) {
            op_cost         = new_cost;
            op_first        = first;
            op_second       = second;
            op_first_next   = first_next;
            op_second_next  = second_next;
        }
      }
    }

    if (op_cost == cost) return *this;

    return solution(change_path(op_first, op_first_next, op_second, op_second_next), op_cost);
  }

 private:
  solution two_opt() {
    int first;
    int first_next;
    int second;
    int second_next;
    double new_cost;

    int n = 500;
    do {
      do {
        first       = (int) rand() % size;
        first_next  = first + 1 == size ? 0 : first + 1;
        second      = ((int) rand() % (size - 3) + 2 + first) % size;
        second_next = second + 1 == size ? 0 : second + 1;

      } while (first == second);

      new_cost = cost - Graph[path[first]][path[first_next]] - Graph[path[second]][path[second_next]]
          + Graph[path[first]][path[second]] + Graph[path[first_next]][path[second_next]];

      if (new_cost < cost) break;
    } while (n--);

    if (n <= 0) return *this;

    return solution(change_path(first, first_next, second, second_next), new_cost);
  }

  solution two_opt_rand() {
    int first       = (int) rand() % size;
    int first_next  = first + 1 == size ? 0 : first + 1;
    int second      = ((int) rand() % (size - 3) + 2 + first) % size;
    int second_next = second + 1 == size ? 0 : second + 1;
    double new_cost;

    new_cost = cost - Graph[path[first]][path[first_next]] - Graph[path[second]][path[second_next]]
        + Graph[path[first]][path[second]] + Graph[path[first_next]][path[second_next]];

    return solution(change_path(first, first_next, second, second_next), new_cost);
  }

  vector<int> change_path(int first, int first_next, int second, int second_next) {
    vector<int> p;
    int i = 0;

    do { p.push_back(path[i]); i++; }
    while (i != first_next && i != second_next);

    if (i == first_next) {
      i = second;

      while (i != first)
      { p.push_back(path[i]); i--; }

      i = second_next;

      while (i < size && i != 0)
      { p.push_back(path[i]); i++; }

    } else if (i == second_next) {
      i = first;

      while (i != second)
      { p.push_back(path[i]); i--; }

      i = first_next;

      while (i < size && i != 0)
      { p.push_back(path[i]); i++; }
    }

    return p;
  }

 public:
  auto iterated_local_search() {
    auto s(*this);
    s = s.local_search();

    int count = 0;
    do {
      auto s1 = s.perturbation();
      s1 = s1.local_search();

      if (s1.cost < s.cost - 1) {
        s = s1;
        std::cout << s1.cost << '\n';
        count = 0;
      } else { count++; }

    } while (count < 20);

    return s;
  }

};  // struct solution


}


int main() {
  srand(time(nullptr));
  tsp::Graph = parse_file("ja_1000.txt");

  tsp::solution s;
  s.initialize();
  int n = 400;
  do {
    auto s1 = s.perturbation_rand();
    s1 = s1.perturbation_rand();
    s1 = s1.perturbation_rand();
    s1 = s1.perturbation_rand();
    s1 = s1.iterated_local_search();
    if (s1.cost < s.cost) {
      s = s1;
    }
    std::cout << ' ' << n << '\n';
  } while (n--);

  stringstream ss;
  std::cout << "Path: ";
  for (auto r : s.path)
    ss << r + 1 << ' ';

  cout << ss.str();

  ofstream f("output.txt");
  f << ss.str();
  f.close();

  std::cout << std::endl << "Cost: " << s.cost << std::endl;

  for (int i = 0; i < size; i++)
    delete[] tsp::Graph[i];
  delete[] tsp::Graph;
}
