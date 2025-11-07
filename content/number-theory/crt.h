#ifndef CRT_H
#define CRT_H

#include <numeric>
#include <utility>
#include <vector>

namespace CRT {
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

// Solves x = a_i (mod m_i). Returns {x, LCM(m_i)}.
// If no solution, returns {-1, -1}.
// Works for non-coprime moduli.
std::pair<ll, ll> crt(const std::vector<ll> &a, const std::vector<ll> &m) {
  ll x = 0, M = 1;
  if (a.empty())
    return {0, 1};
  x = a[0];
  M = m[0];
  for (size_t i = 1; i < a.size(); ++i) {
    ll a_i = a[i], m_i = m[i];
    ll c = (a_i - x % m_i + m_i) % m_i;
    ll x_i, y_i;
    ll g = exEuclid(M, m_i, x_i, y_i);
    if (c % g != 0)
      return {-1, -1}; // No solution
    ll k = mult(c / g, x_i, m_i / g);
    x = x + k * M;
    M = std::lcm(M, m_i);
    x = (x % M + M) % M;
  }
  return {x, M};
}
} // namespace CRT
#endif