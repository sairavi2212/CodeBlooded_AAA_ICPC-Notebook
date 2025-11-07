#include <bits/stdc++.h>
using namespace std;

/*
  FenwickRange<T>
  - 0-based public indexing for callers
  - Internally uses 1-based Fenwick arrays
  - Methods:
      FenwickRange(int n)              : create size n (elements indexed 0..n-1)
      void range_add(int l, int r, T v): add v to every element in [l, r]
  (0-based inclusive) T prefix_sum(int idx)            : sum of elements
  [0..idx] (0-based). idx < 0 -> 0 T range_sum(int l, int r)        : sum of
  elements [l..r] (0-based) T point_query(int idx)           : value at index
  idx (0-based)
*/
template <typename T = long long> class FenwickRange {
public:
  explicit FenwickRange(int n = 0)
      : n_(n), B1(n_ + 1, T(0)), B2(n_ + 1, T(0)) {}

  int size() const noexcept { return n_; }

  // add val to every element in [l, r] (0-based indices)
  void range_add(int l, int r, T val) {
    if (n_ == 0)
      return;
    if (l < 0)
      l = 0;
    if (r >= n_)
      r = n_ - 1;
    if (l > r)
      return;
    int L = l + 1; // convert to 1-based
    int R = r + 1;
    add(B1, L, val);
    add(B1, R + 1, -val);
    add(B2, L, val * T(L - 1));
    add(B2, R + 1, -val * T(R));
  }

  // sum of [0..idx] (0-based). if idx<0 -> 0
  T prefix_sum(int idx) const {
    if (idx < 0)
      return T(0);
    if (idx >= n_)
      idx = n_ - 1;
    int i = idx + 1; // 1-based
    return sum(B1, i) * T(i) - sum(B2, i);
  }

  // sum of [l..r] (0-based)
  T range_sum(int l, int r) const {
    if (l > r)
      return T(0);
    return prefix_sum(r) - prefix_sum(l - 1);
  }

  // value at single index (0-based)
  T point_query(int idx) const { return range_sum(idx, idx); }

  void reset() {
    std::fill(B1.begin(), B1.end(), T(0));
    std::fill(B2.begin(), B2.end(), T(0));
  }

private:
  int n_;
  vector<T> B1, B2; // 1..n

  static void add(vector<T> &bit, int idx1, T val) {
    int n = (int)bit.size() - 1;
    for (int i = idx1; i <= n; i += (i & -i))
      bit[i] += val;
  }

  static T sum(const vector<T> &bit, int idx1) {
    if (idx1 <= 0)
      return T(0);
    int i = idx1;
    T res = T(0);
    for (; i > 0; i -= (i & -i))
      res += bit[i];
    return res;
  }
};
