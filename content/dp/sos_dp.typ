== sos_dp.h
```cpp
// SOS DP: f[mask] = sum of f[submask] for all submask ⊆ mask
void sos_dp(vector<long long> &f, int n) {
  for (int i = 0; i < n; i++)
    for (int mask = 0; mask < (1 << n); mask++)
      if (mask & (1 << i))
        f[mask] += f[mask ^ (1 << i)];
}

// SOSU DP: f[mask] = sum of f[supmask] for all supmask ⊇ mask
void sosu_dp(vector<long long> &f, int n) {
  for (int i = 0; i < n; i++)
    for (int mask = 0; mask < (1 << n); mask++)
      if (!(mask & (1 << i)))
        f[mask] += f[mask | (1 << i)];
}
```