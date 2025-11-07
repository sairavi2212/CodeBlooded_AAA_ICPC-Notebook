== longest_common_prefix.h
```cpp
#include <bits/stdc++.h>
using namespace std;

// --- suffix array (sort_cyclic_shifts + suffix_array) ---
vector<int> sort_cyclic_shifts(const string &a) {
  int n = a.size();
  const int alphabet = 256;

  vector<int> p(n), c(n);
  vector<int> cnt(max(alphabet, n), 0);

  for (int i = 0; i < n; i++)
    cnt[(unsigned char)a[i]]++;
  for (int i = 1; i < alphabet; i++)
    cnt[i] = cnt[i - 1] + cnt[i];
  for (int i = n - 1; i >= 0; i--)
    p[--cnt[(unsigned char)a[i]]] = i;

  c[p[0]] = 0;
  int classes = 1;
  for (int i = 1; i < n; i++) {
    if (a[p[i]] != a[p[i - 1]])
      classes++;
    c[p[i]] = classes - 1;
  }

  for (int h = 0; (1 << h) < n; h++) {
    vector<int> pn(n), cn(n);
    for (int i = 0; i < n; i++) {
      pn[i] = p[i] - (1 << h);
      if (pn[i] < 0)
        pn[i] += n;
    }
    fill(cnt.begin(), cnt.begin() + classes, 0);
    for (int i = 0; i < n; i++)
      cnt[c[pn[i]]]++;
    for (int i = 1; i < classes; i++)
      cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; i--)
      p[--cnt[c[pn[i]]]] = pn[i];

    cn[p[0]] = 0;
    classes = 1;
    for (int i = 1; i < n; i++) {
      pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
      pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << h)) % n]};
      if (cur != prev)
        classes++;
      cn[p[i]] = classes - 1;
    }
    c.swap(cn);
  }

  return p;
}

vector<int> suffix_array(string s) {
  s += '$';
  vector<int> sorted_shifts = sort_cyclic_shifts(s);
  // remove sentinel position
  sorted_shifts.erase(sorted_shifts.begin());
  return sorted_shifts;
}

// --- Kasai LCP construction ---
vector<int> lcp_construction(const string &s, const vector<int> &p) {
  int n = (int)s.size();
  vector<int> rank(n, 0);
  for (int i = 0; i < n; i++)
    rank[p[i]] = i;

  int k = 0;
  vector<int> lcp(max(0, n - 1), 0);
  for (int i = 0; i < n; i++) {
    if (rank[i] == n - 1) {
      k = 0;
      continue;
    }
    int j = p[rank[i] + 1];
    while (i + k < n && j + k < n && s[i + k] == s[j + k])
      k++;
    lcp[rank[i]] = k;
    if (k)
      k--;
  }
  return lcp;
}

// --- helpers: rank from SA and sparse table (RMQ) over lcp[] ---
vector<int> build_rank_from_sa(const vector<int> &p) {
  int n = (int)p.size();
  vector<int> rank(n, 0);
  for (int i = 0; i < n; ++i)
    rank[p[i]] = i;
  return rank;
}

vector<vector<int>> build_rmq(const vector<int> &lcp) {
  int m = (int)lcp.size();
  if (m == 0)
    return {};
  int LOG = 1;
  while ((1 << LOG) <= m)
    ++LOG;
  vector<vector<int>> st(LOG, vector<int>(m));
  st[0] = lcp;
  for (int k = 1; k < LOG; ++k) {
    int len = 1 << k;
    for (int i = 0; i + len <= m; ++i) {
      st[k][i] = min(st[k - 1][i], st[k - 1][i + (len >> 1)]);
    }
  }
  return st;
}

int rmq_query(const vector<vector<int>> &st, int l, int r) {
  if (st.empty())
    return 0;
  int len = r - l + 1;
  int k = 31 - __builtin_clz(len);
  return min(st[k][l], st[k][r - (1 << k) + 1]);
}

// --- LCP class ---
struct LCP {
  string s;
  int n = 0;
  vector<int> sa;         // suffix array
  vector<int> lcp;        // kasai lcp array (size n-1)
  vector<int> rank;       // rank[i] = position of suffix i in sa
  vector<vector<int>> st; // sparse table on lcp[]

  // construct from a string (builds SA, LCP, rank and RMQ)
  LCP(const string &str = "") {
    if (!str.empty())
      build(str);
  }

  void build(const string &str) {
    s = str;
    n = (int)s.size();
    if (n == 0) {
      sa.clear();
      lcp.clear();
      rank.clear();
      st.clear();
      return;
    }
    sa = suffix_array(s);          // size n
    lcp = lcp_construction(s, sa); // size n-1 (or 0 if n==1)
    rank = build_rank_from_sa(sa);
    st = build_rmq(lcp);
  }

  // answer LCP of suffixes starting at i and j (0-indexed positions in original
  // string)
  int query(int i, int j) const {
    if (i < 0 || j < 0 || i >= n || j >= n)
      return 0; // invalid -> 0
    if (i == j)
      return n - i; // full suffix matches itself
    int ri = rank[i], rj = rank[j];
    if (ri > rj)
      swap(ri, rj);
    // LCP of suffixes at sa[ri] and sa[rj] is min over lcp[ri..rj-1]
    return rmq_query(st, ri, rj - 1);
  }

  // ---------------------------
  // Number of different substrings
  // ---------------------------
  // Explanation:
  // For a string of length n, total number of substrings (including duplicates)
  // is:
  //   total = n * (n + 1) / 2
  //
  // When suffixes are sorted (by SA), each suffix p[i] contributes
  //   (n - p[i]) - lcp[i-1]
  // new distinct substrings (prefixes that weren't counted earlier).
  // Summing over i gives:
  //
  // distinct = sum_{i=0..n-1} (n - p[i]) - sum_{i=0..n-2} lcp[i]
  // which simplifies to:
  // distinct = n*(n+1)/2 - sum_{i=0..n-2} lcp[i]
  //
  // We implement the compact formula below:
  long long distinct_substrings() const {
    if (n == 0)
      return 0;
    long long total = 1LL * n * (n + 1) / 2;
    long long sum_lcp = 0;
    for (int v : lcp)
      sum_lcp += v;
    return total - sum_lcp;
  }

  // optional accessors
  const vector<int> &get_sa() const { return sa; }
  const vector<int> &get_lcp_array() const { return lcp; }
};

```