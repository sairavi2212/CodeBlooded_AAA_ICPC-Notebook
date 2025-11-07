== primroot.h
```cpp
#ifndef PRIMITIVE_ROOT_H
#define PRIMITIVE_ROOT_H

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <vector>

namespace PrimitiveRoot {
using ll = long long;
using u128 = __int128;

inline ll mult(ll a, ll b, ll mod) { return (ll)((u128)a * b % mod); }

ll power(ll a, ll b, ll m) {
  ll res = 1;
  a %= m;
  while (b > 0) {
    if (b & 1)
      res = mult(res, a, m);
    a = mult(a, a, m);
    b >>= 1;
  }
  return res;
}

bool miller_rabin(ll n, int a, ll d, int s) {
  ll x = power(a, d, n);
  if (x == 1 || x == n - 1)
    return true;
  for (int r = 1; r < s; r++) {
    x = mult(x, x, n);
    if (x == n - 1)
      return true;
  }
  return false;
}

bool is_prime(ll n) {
  if (n < 2 || (n % 2 == 0 && n > 2))
    return false;
  if (n == 2 || n == 3)
    return true;
  ll d = n - 1;
  int s = 0;
  while ((d & 1) == 0) {
    d >>= 1;
    s++;
  }
  static const std::array<int, 12> bases = {2,  3,  5,  7,  11, 13,
                                            17, 19, 23, 29, 31, 37};
  for (int a : bases) {
    if (n <= a)
      break;
    if (!miller_rabin(n, a, d, s))
      return false;
  }
  return true;
}

ll brent(ll n, ll x0 = 2, ll c = 1) {
  ll x = x0;
  ll g = 1;
  ll q = 1;
  ll xs, y;
  int m = 128;
  int l = 1;
  while (g == 1) {
    y = x;
    for (int i = 1; i < l; i++)
      x = (mult(x, x, n) + c) % n;
    int k = 0;
    while (k < l && g == 1) {
      xs = x;
      for (int i = 0; i < m && i < l - k; i++) {
        x = (mult(x, x, n) + c) % n;
        q = mult(q, std::abs(y - x), n);
      }
      g = std::gcd(q, n);
      k += m;
    }
    l *= 2;
  }
  if (g == n) {
    do {
      xs = (mult(xs, xs, n) + c) % n;
      g = std::gcd(std::abs(xs - y), n);
    } while (g == 1);
  }
  return g;
}

void factorize_recursive(ll n, std::vector<ll> &factors) {
  if (n == 1)
    return;
  if (is_prime(n)) {
    factors.push_back(n);
    return;
  }
  ll g = n;
  for (ll c = 1; g == n; c++) {
    g = brent(n, 2, c);
  }
  factorize_recursive(g, factors);
  factorize_recursive(n / g, factors);
}

std::vector<ll> factorize(ll n) {
  std::vector<ll> factors;
  factorize_recursive(n, factors);
  std::sort(factors.begin(), factors.end());
  return factors;
}

// Finds the smallest primitive root modulo p.
ll primitive_root(ll p) {
  if (!is_prime(p))
    return -1;
  ll phi = p - 1;
  std::vector<ll> factors = factorize(phi);
  factors.erase(std::unique(factors.begin(), factors.end()), factors.end());
  for (ll g = 2; g <= p; g++) {
    bool ok = true;
    for (ll f : factors) {
      if (power(g, phi / f, p) == 1) {
        ok = false;
        break;
      }
    }
    if (ok)
      return g;
  }
  return -1;
}
} // namespace PrimitiveRoot
#endif
```