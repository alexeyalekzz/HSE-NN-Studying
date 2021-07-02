#include <iostream>
#include <list>
#include <string_view>

uint64_t hash(std::string_view s, uint64_t q, uint64_t d) {
  uint64_t res = 0;
  for (char i : s) {
    res = (d * res + i) % q;
  }
  return res;
}

using ListPair = std::pair<std::list<uint64_t>, std::list<uint64_t>>;

ListPair RKM(std::string_view T, std::string_view P, int mod) {
  std::list<uint64_t> matches, s_hits;
  size_t n = T.length(), m = P.length();
  int d = 26;
  uint64_t q = mod, T_hashed, P_hashed = hash(P, q, d);

  for (size_t s = 0; s <= n - m; ++s) {
    T_hashed = hash(T.substr(s, m), q, d);

    if (P_hashed == T_hashed) {
      if (T.substr(s, m) == P) {
        matches.push_back(s);
      } else {
        s_hits.push_back(s);
      }
    }
  }

  return { matches, s_hits };
}

std::ostream& operator<<(std::ostream& out, const std::list<uint64_t>& l1) {
  for (auto elem : l1) {
    out << elem << ' ';
  }
  return out;
}

int main() {
  std::string T, P;
  int m;
  std::cin >> m >> P >> T;

  auto [matches, spurious_hits] = RKM(T, P, m);

  std::cout << "Matches: " << matches << std::endl
            << "Spurious hits: " << spurious_hits << std::endl;

  return 0;
}
