#ifndef DISCRETE_ROOT_H
#define DISCRETE_ROOT_H

#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <array>
#include <map>

namespace DiscreteRoot {
    using ll = long long;
    using u128 = __int128;

    // --- Core Dependencies ---
    inline ll mult(ll a, ll b, ll mod) { return (ll)((u128)a * b % mod); }

    ll power(ll a, ll b, ll m) {
        ll res = 1; a %= m;
        while (b > 0) {
            if (b & 1) res = mult(res, a, m);
            a = mult(a, a, m);
            b >>= 1;
        }
        return res;
    }

    ll exEuclid(ll a, ll b, ll &x, ll &y) {
        if (b == 0) { x = 1; y = 0; return a; }
        ll x1, y1;
        ll d = exEuclid(b, a % b, x1, y1);
        x = y1;
        y = x1 - y1 * (a / b);
        return d;
    }

    // --- Primality & Factorization Dependencies ---
    bool miller_rabin(ll n, int a, ll d, int s) {
        ll x = power(a, d, n);
        if (x == 1 || x == n - 1) return true;
        for (int r = 1; r < s; r++) {
            x = mult(x, x, n);
            if (x == n - 1) return true;
        }
        return false;
    }

    bool is_prime(ll n) {
        if (n < 2 || (n % 2 == 0 && n > 2)) return false;
        if (n == 2 || n == 3) return true;
        ll d = n - 1; int s = 0;
        while ((d & 1) == 0) { d >>= 1; s++; }
        static const std::array<int, 12> bases = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
        for (int a : bases) {
            if (n <= a) break;
            if (!miller_rabin(n, a, d, s)) return false;
        }
        return true;
    }

    ll brent(ll n, ll x0=2, ll c=1) {
        ll x = x0; ll g = 1; ll q = 1; ll xs, y;
        int m = 128; int l = 1;
        while (g == 1) {
            y = x;
            for (int i = 1; i < l; i++) x = (mult(x, x, n) + c) % n;
            int k = 0;
            while (k < l && g == 1) {
                xs = x;
                for (int i = 0; i < m && i < l - k; i++) {
                    x = (mult(x, x, n) + c) % n;
                    q = mult(q, std::abs(y - x), n);
                }
                g = std::gcd(q, n); k += m;
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
        if (n == 1) return;
        if (is_prime(n)) { factors.push_back(n); return; }
        ll g = n;
        for (ll c = 1; g == n; c++) { g = brent(n, 2, c); }
        factorize_recursive(g, factors);
        factorize_recursive(n / g, factors);
    }

    std::vector<ll> factorize(ll n) {
        std::vector<ll> factors;
        factorize_recursive(n, factors);
        std::sort(factors.begin(), factors.end());
        return factors;
    }

    // --- Primitive Root Dependency ---
    ll primitive_root(ll p) {
        if (!is_prime(p)) return -1;
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
            if (ok) return g;
        }
        return -1;
    }

    // --- Discrete Log Dependency ---
    ll bsgs(ll a, ll b, ll m) {
        a %= m; b %= m;
        if (b == 1 || m == 1) return 0;
        ll n = (ll)std::sqrt(m) + 1;
        std::map<ll, ll> giant_steps;
        ll an = 1;
        for (ll i = 0; i < n; i++) an = mult(an, a, m);
        for (ll q = 0, cur = b; q <= n; q++) {
            giant_steps[cur] = q;
            cur = mult(cur, a, m);
        }
        for (ll p = 1, cur = 1; p <= n; p++) {
            cur = mult(cur, an, m);
            if (giant_steps.count(cur)) {
                ll x = p * n - giant_steps[cur];
                if (power(a, x, m) == b) return x;
            }
        }
        return -1;
    }

    // --- Discrete Root ---
    // Solves x^k = a (mod p) for prime p.
    ll discrete_root(ll k, ll a, ll p) {
        if (a == 0) return 0;
        ll g = primitive_root(p);
        if (g == -1) return -1; // Not prime
        ll b = bsgs(g, a, p);
        if (b == -1) return -1; // a is not in the group
        ll x_0, y_0;
        ll d = exEuclid(k, p - 1, x_0, y_0);
        if (b % d != 0) return -1;
        ll x = mult(x_0, b / d, (p - 1) / d);
        x = (x % ((p-1)/d) + ((p-1)/d)) % ((p-1)/d);
        return power(g, x, p);
    }
}
#endif