== factorization.h
```cpp
namespace NT {
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

// Deterministic Miller-Rabin for 64-bit integers
bool is_prime(ll n) {
  if (n < 2)
    return false;
  if (n == 2 || n == 3)
    return true;
  if (n % 2 == 0 || n % 3 == 0)
    return false;
  ll d = n - 1;
  int s = 0;
  while ((d & 1) == 0) {
    d >>= 1;
    s++;
  }
  static const array<int, 12> bases = {2,  3,  5,  7,  11, 13,
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
        q = mult(q, abs(y - x), n);
      }
      g = gcd(q, n);
      k += m;
    }
    l *= 2;
  }
  if (g == n) {
    do {
      xs = (mult(xs, xs, n) + c) % n;
      g = gcd(abs(xs - y), n);
    } while (g == 1);
  }
  return g;
}

void factorize_recursive(ll n, vector<ll> &factors) {
  if (n == 1)
    return;
  // Trial division for small primes can speed up standard cases
  if (n < 1000) {
    for (ll p : {2, 3, 5}) {
      while (n % p == 0) {
        factors.push_back(p);
        n /= p;
      }
    }
    if (n == 1)
      return;
  }
  if (is_prime(n)) {
    factors.push_back(n);
    return;
  }
  ll g = n;
  // Retry brent with different params if it fails
  for (ll c = 1; g == n; c++) {
    g = brent(n, 2, c);
  }
  factorize_recursive(g, factors);
  factorize_recursive(n / g, factors);
}

// Main function to call
vector<ll> factorize(ll n) {
  vector<ll> factors;
  factorize_recursive(n, factors);
  sort(factors.begin(), factors.end());
  return factors;
}
} // namespace NT
```