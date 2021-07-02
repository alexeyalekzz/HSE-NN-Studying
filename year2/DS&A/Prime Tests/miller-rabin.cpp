#include <iostream>
#include <cmath>
#include <tuple>

auto miller_rabin_test(int64_t n) {
  int64_t f = 0, c1 = 0, c2 = 0, s = 0, d = n - 1;

  while (d % 2 == 0) {
    d /= 2;
    s++;
  }

  for (int64_t i = 1; i < n; ++i) {
    int64_t x = exp_modulo(i, d, n);
    if (x == 1) {
      c1++;
      continue;
    }

    for (int64_t j = 0; j < s; ++j) {
      x = exp_modulo(i, (int64_t) powl(2, j) * d, n);
      if (x == n - 1) {
        c2++;
        break;
      } else if (j == s - 1) {
        f++;
        break;
      }
    }
  }

  return std::tuple{c1 + c2 > f, c1, c2, f};
}


int main() {
  int64_t n;
  std::cin >> n;

  auto [res, cond1, cond2, failed] = miller_rabin_test(n);

  std::cout << "Miller-Rabin test: "
            << (res ? "True " : "False ")
            << failed << ' '
            << cond1 << ' '
            << cond2 << std::endl;

  return 0;
}
