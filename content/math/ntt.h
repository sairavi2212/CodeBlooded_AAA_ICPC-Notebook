#ifndef NTT_H
#define NTT_H

#include <iostream>
#include <vector>
#include <algorithm> // For std::swap

namespace NTT {

    using ll = long long;

    const ll MOD = 998244353;
    const ll ROOT = 3;

    ll power(ll a, ll p) {
        ll res = 1;
        a %= MOD;
        while (p > 0) {
            if (p & 1) res = (res * a) % MOD;
            a = (a * a) % MOD;
            p >>= 1;
        }
        return res;
    }

    ll modInverse(ll a) {
        return power(a, MOD - 2);
    }

    void ntt(std::vector<ll>& a, bool invert) {
        int n = a.size();
        if (n == 1) return;

        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) {
                j ^= bit;
            }
            j ^= bit;
            if (i < j) {
                std::swap(a[i], a[j]);
            }
        }

        for (int len = 2; len <= n; len <<= 1) {
            ll wlen = power(ROOT, (MOD - 1) / len);
            if (invert) {
                wlen = modInverse(wlen);
            }
            for (int i = 0; i < n; i += len) {
                ll w = 1;
                for (int j = 0; j < len / 2; j++) {
                    ll u = a[i + j];
                    ll v = (a[i + j + len / 2] * w) % MOD;
                    a[i + j] = (u + v) % MOD;
                    a[i + j + len / 2] = (u - v + MOD) % MOD;
                    w = (w * wlen) % MOD;
                }
            }
        }

        if (invert) {
            ll n_inv = modInverse(n);
            for (ll& x : a) {
                x = (x * n_inv) % MOD;
            }
        }
    }

    std::vector<ll> poly_multiply_ntt(std::vector<ll> a, std::vector<ll> b) {
        int res_size = a.size() + b.size() - 1;
        int n = 1;
        while (n < res_size) {
            n <<= 1;
        }

        a.resize(n);
        b.resize(n);

        ntt(a, false);
        ntt(b, false);

        for (int i = 0; i < n; i++) {
            a[i] = (a[i] * b[i]) % MOD;
        }

        ntt(a, true);
        
        a.resize(res_size); // Truncate to the correct polynomial size
        return a;
    }

} // namespace NTT

#endif // NTT_H