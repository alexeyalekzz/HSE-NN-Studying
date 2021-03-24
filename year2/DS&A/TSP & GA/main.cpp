#include <iostream>
#include "GeneticTSP.h"


/**********************         Cromosome        ************************/
double Genetic::cost(const chromosome& s) const {
  double res = 0;
  for (int i = 1; i < SIZE; ++i)
    res += data[s[i - 1]][s[i]];
  res += data[s[0]][s[SIZE - 1]];
  return res;
}

void Genetic::init(chromosome& s) {
  static std::mt19937 gen(time(nullptr));
  s.resize(SIZE);
  for (int i{0}; i < SIZE; ++i)
    s[i] = i;
  std::shuffle(s.begin(), s.end(), gen);
}

void Genetic::eval(chromosome& s) const {
  double initial_cost = cost(s);
  double op_cost = initial_cost;
  int op_first;
  int op_first_next;
  int op_second;
  int op_second_next;
  long n = 1000;
  while (n--) {
    int first       = (int) rand() % SIZE;
    int first_next  = first + 1 == SIZE ? 0 : first + 1;
    int second      = ((int) rand() % (SIZE - 3) + 2 + first) % SIZE;
    int second_next = second + 1 == SIZE ? 0 : second + 1;

    double new_cost;
    if (first != second) {
      new_cost = initial_cost - data[s[first]][s[first_next]] - data[s[second]][s[second_next]]
          + data[s[first]][s[second]] + data[s[first_next]][s[second_next]];

      if (new_cost < op_cost) {
        op_cost         = new_cost;
        op_first        = first;
        op_second       = second;
        op_first_next   = first_next;
        op_second_next  = second_next;
      }
    }
  }

  if ((initial_cost - op_cost) <= EPS) return;

  chromosome p;
  int i = 0;

  do { p.push_back(s[i]); i++; }
  while (i != op_first_next && i != op_second_next);

  if (i == op_first_next) {
    i = op_second;

    while (i != op_first)
    { p.push_back(s[i]); i--; }

    i = op_second_next;

    while (i < SIZE && i != 0)
    { p.push_back(s[i]); i++; }

  } else if (i == op_second_next) {
    i = op_first;

    while (i != op_second)
    { p.push_back(s[i]); i--; }

    i = op_first_next;

    while (i < SIZE && i != 0)
    { p.push_back(s[i]); i++; }
  }

  s = std::move(p);
}

void Genetic::mutate(chromosome& s) {
  if (rand() % 100 <= MUT_PROBABILITY) {
    int p1, p2;
    do {
      p1 = rand() % SIZE;
      p2 = rand() % SIZE;

    } while (p1 == p2);

    auto temp = s[p1];
    s[p1] = s[p2];
    s[p2] = temp;
  }
}

void Genetic::cycle_crossover(const chromosome& p1, const chromosome& p2, chromosome& c1, chromosome& c2) {
  int n = 0;
  c1.resize(SIZE, -1);
  c2.resize(SIZE, -1);

  do {
    int t = p2[n];
    c1[n] = p1[n];
    c2[n] = t;
    n = std::find(p1.begin(), p1.end(), t) - p1.begin();
  } while (n);

  for (int i = 0; i < SIZE; ++i) {
    if (c1[i] == -1) c1[i] = p2[i];
    if (c2[i] == -1) c2[i] = p1[i];
  }
}
/**********************         Cromosome        ************************/



/************************         Group        **************************/
template<size_t size>
void Genetic::modification(group<size>& g) {
  for (auto& ch : g)
    mutate(ch);
}

template<size_t size>
void Genetic::evaluation(group<size>& g) const {
  for (auto& ch : g)
    eval(ch);
}

void Genetic::initialize(group<POP_SIZE>& g) {
  for (auto& ch : g)
    init(ch);
}

group<POP_SIZE> Genetic::build_next_delete_all(group<POP_SIZE>& p, group<CHILDREN_N>& c) {
  static std::mt19937 gen(time(nullptr));
  group<POP_SIZE> res;
  chromosome* arr[CHILDREN_N];

  for (int i = 0; i < CHILDREN_N; ++i)
    arr[i] = &c[i];

  std::shuffle(arr, arr + CHILDREN_N, gen);

  for (int i = 0; i < POP_SIZE; ++i)
    res[i] = std::move(*arr[i]);

  return res;
}

group<CHILDREN_N> Genetic::reproduction(const group<POP_SIZE>& g) const {
  int c = 0;
  group<CHILDREN_N> childrens;
  group<PARENTS_N> parents = tournament_selection(g);

  for (int i = 0; i < PARENTS_N; ++i) {
    for (int j = i + 1; j < PARENTS_N; ++j) {
      Genetic::cycle_crossover(parents[i], parents[j], childrens[c], childrens[c + 1]);
      c += 2;
    }
  }

  return childrens;
}

group<PARENTS_N> Genetic::tournament_selection(const group<POP_SIZE>& g) const {
  const int SELECTION_SIZE = 4;
  const chromosome* selection[SELECTION_SIZE];
  group<PARENTS_N> result;

  for (auto& ch : result) {
    ch.resize(SIZE);
    for (auto& selected_ch : selection)
      selected_ch = &g[rand() % POP_SIZE];

    int indx = 0;
    double best_cost = cost(*selection[0]);
    for (int i{1}; i < SELECTION_SIZE; ++i) {
      if (auto c = cost(*selection[i]); c > best_cost + EPS) {
        indx = i;
        best_cost = c;
      }
    }
    ch = *selection[indx];
  }

  return result;
}

chromosome Genetic::find_best(const group<POP_SIZE>& g) const {
  auto best = g[0];
  for (auto& ch : g)
    if (cost(ch) > cost(best) + EPS)
      best = ch;
  return best;
}
/***********************        Group        ***************************/


chromosome Genetic::start_genetics() const {
  constexpr int max_iters = 10000;
  size_t iters = 0;
  group<POP_SIZE> population;

  Genetic::initialize(population);
  Genetic::evaluation(population);

  chromosome best = population[0];

  while (iters < max_iters) {
    std::cout << cost(find_best(population)) << std::endl;

    group<CHILDREN_N> childrens = reproduction(population);
    Genetic::modification(childrens);
    Genetic::evaluation(childrens);
    population = Genetic::build_next_delete_all(population, childrens);
    // population = Genetic::build_next_steady_state(population, children);
    // population = Genetic::build_next_steady_state_no_dup(population, children);

    if (auto c = find_best(population); cost(c) < cost(best)) {
      best = c;
      std::cout << cost(c) << std::endl;
    }

    iters++;
  }

  return best;
}

int main() {
  Genetic problem("data.txt");

  std::cout << "Started\n";
  chromosome solution = problem.start_genetics();

  return 0;
}
