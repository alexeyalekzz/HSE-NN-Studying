#include <iostream>
#include <cmath>
#include <tuple>

auto fermat_test(int64_t n) { 
  int64_t c, f;

  for (long i = 1; i < n; i++) {
    if (exp_modulo(i, n - 1, n) == 1) {
      c++;
    } else {
      f++;
    }
  }

  return std::tuple{c > f, c, f};
}

int main() {
  int64_t n;
  std::cin >> n;

  auto [res, cond, failed] = fermat_test(n);

  std::cout << "Fermat test: "
            << (res ? "True " : "False ")
            << failed << ' '
            << cond1 << std::endl;

  return 0;
}
