== discretelog.h
```cpp
#ifndef DISCRETE_LOG_H
#define DISCRETE_LOG_H

#include <cmath>
#include <map>
#include <numeric>

namespace DiscreteLog {
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

// Solves a^x = b (mod m). Returns x, or -1 if no solution.
// Assumes gcd(a, m) = 1.
ll bsgs(ll a, ll b, ll m) {
  a %= m;
  b %= m;
  if (b == 1 || m == 1)
    return 0;
  ll n = (ll)std::sqrt(m) + 1;
  std::map<ll, ll> giant_steps;
  ll an = 1;
  for (ll i = 0; i < n; i++)
    an = mult(an, a, m);

  for (ll q = 0, cur = b; q <= n; q++) {
    giant_steps[cur] = q;
    cur = mult(cur, a, m);
  }

  for (ll p = 1, cur = 1; p <= n; p++) {
    cur = mult(cur, an, m);
    if (giant_steps.count(cur)) {
      ll x = p * n - giant_steps[cur];
      if (power(a, x, m) == b)
        return x;
    }
  }
  return -1;
}
} // namespace DiscreteLog
#endif
```