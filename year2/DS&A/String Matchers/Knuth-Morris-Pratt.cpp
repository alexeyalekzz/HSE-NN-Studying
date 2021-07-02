#include <iostream>
#include <vector>

std::vector<size_t> compute_prefix_fun(std::string_view P) {
  size_t m = P.length();
  std::vector<size_t> Pi(m + 1);
  size_t k = 0;

  for (size_t q = 2; q <= m; ++q) {
    while (k > 0 && P[k] != P[q - 1]) {
      k = Pi[k];
    }
    if (P[k] == P[q - 1]) {
      k = k + 1;
    }
    Pi[q] = k;
  }

  return Pi;
}

std::vector<size_t> KMP_matcher(std::string_view T, std::string_view P) {
  size_t n = T.length(), m = P.length();
  std::vector<size_t> res;

  auto Pi = compute_prefix_fun(P);
  size_t q = 0;

  for (size_t i = 0; i < n; ++i) {
    while (q > 0 && P[q] != T[i]) {
      q = Pi[q];
    }

    if (P[q] == T[i]) {
      q = q + 1;
    }

    if (q == m) {
      res.push_back((i + 1) - m);
      q = Pi[q];
    }
  }

  return res;
}

std::ostream& operator<<(std::ostream& out, const std::vector<uint64_t>& v) {
  for (int i = 0; i < v.size(); ++i) {
    out << v[i];
    if (i != v.size() - 1) {
      out << ' ';
    }
  }
  return out;
}

int main() {
  std::string T, P;
  std::cin >> P >> T;

  auto result = compute_prefix_fun(P);
  std::cout << "Prefix template: "
            << std::vector(result.data() + 1, result.data() + result.size())
            << std::endl;

  result = compute_prefix_fun(P + '0' + T);
  std::cout << "Prefix function: "
            << std::vector(result.data() + 1, result.data() + result.size())
            << std::endl;

  result = KMP_matcher(T, P);
  std::cout << "Matches: "
            << result
            << std::endl;

  return 0;
}
