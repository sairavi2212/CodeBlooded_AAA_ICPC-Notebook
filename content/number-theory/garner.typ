== garner.h
```cpp
#ifndef GARNER_H
#define GARNER_H

#include <numeric>
#include <vector>

namespace Garner {
using ll = long long;
using u128 = __int128;

inline ll mult(ll a, ll b, ll mod) { return (ll)((u128)a * b % mod); }

ll exEuclid(ll a, ll b, ll &x, ll &y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  ll x1, y1;
  ll d = exEuclid(b, a % b, x1, y1);
  x = y1;
  y = x1 - y1 * (a / b);
  return d;
}

ll modInverse(ll a, ll m) {
  ll x, y;
  ll g = exEuclid(a, m, x, y);
  if (g != 1)
    return -1;
  return (x % m + m) % m;
}

// Solves x = a_i (mod m_i) for *coprime* m_i.
ll garner(const std::vector<ll> &a, const std::vector<ll> &m) {
  int n = a.size();
  std::vector<ll> x(n);
  for (int i = 0; i < n; ++i) {
    x[i] = a[i];
    for (int j = 0; j < i; ++j) {
      ll inv = modInverse(m[j], m[i]);
      x[i] = mult((x[i] - x[j] + m[i]), inv, m[i]);
    }
  }
  ll res = x[n - 1];
  // Use a large prime (e.g., 10^18 + 9) or __int128 for M if result overflows
  // ll
  ll M = 1e18 + 9;
  for (int i = n - 2; i >= 0; --i) {
    res = (mult(res, m[i], M) + x[i]) % M;
  }
  return res;
}
} // namespace Garner
#endif
```