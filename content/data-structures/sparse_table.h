
class gcd_sparse_table {
public:
  vector<long long> a;
  int n;
  vector<vector<long long>> sparse_table;

  gcd_sparse_table(vector<long long> &arr) {
    a = arr;
    n = a.size();
    sparse_table.assign(n + 1, vector<long long>(log2(n) + 2));
    build();
  }

  void build() {
    for (int i = 0; i < n; i++)
      sparse_table[i][0] = a[i];
    for (int j = 1; (1 << j) <= n; j++) {
      for (int i = 0; i + (1 << j) - 1 < n; i++) {
        sparse_table[i][j] = __gcd(sparse_table[i][j - 1],
                                   sparse_table[i + (1 << (j - 1))][j - 1]);
      }
    }
  }

  long long query(int l, int r) {
    if (l > r)
      return 0;
    int len = r - l + 1;
    int p = log2(len);
    return __gcd(sparse_table[l][p], sparse_table[r - (1 << p) + 1][p]);
  }
};

class GcdSparseTable2D {
public:
  int n, m, K;
  std::vector<std::vector<long long>> a;
  std::vector<std::vector<std::vector<long long>>> st;

  GcdSparseTable2D(const std::vector<std::vector<long long>> &matrix) {
    a = matrix;
    n = a.size();
    m = n ? a[0].size() : 0;
    K = std::log2(std::min(n, m)) + 1;
    st.assign(
        n, std::vector<std::vector<long long>>(m, std::vector<long long>(K)));
    build();
  }

  void build() {
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < m; ++j)
        st[i][j][0] = a[i][j];

    for (int k = 1; k < K; ++k) {
      int len = 1 << k, hl = len >> 1;
      for (int i = 0; i + len <= n; ++i) {
        for (int j = 0; j + len <= m; ++j) {
          long long g1 = std::gcd(st[i][j][k - 1], st[i + hl][j][k - 1]);
          long long g2 =
              std::gcd(st[i][j + hl][k - 1], st[i + hl][j + hl][k - 1]);
          st[i][j][k] = std::gcd(g1, g2);
        }
      }
    }
  }

  long long query(int x1, int y1, int x2, int y2) {
    if (x1 > x2)
      std::swap(x1, x2);
    if (y1 > y2)
      std::swap(y1, y2);
    int k = std::log2(std::min(x2 - x1 + 1, y2 - y1 + 1));
    int dx = x2 - (1 << k) + 1, dy = y2 - (1 << k) + 1;
    long long t1 = std::gcd(st[x1][y1][k], st[dx][y1][k]);
    long long t2 = std::gcd(st[x1][dy][k], st[dx][dy][k]);
    return std::gcd(t1, t2);
  }
};

class max_sparse_table {
public:
  vector<long long> a;
  int n;
  vector<vector<long long>> sparse_table;

  max_sparse_table(vector<long long> &arr) {
    a = arr;
    n = a.size();
    sparse_table.assign(n + 1, vector<long long>(log2(n) + 2));
    build();
  }

  void build() {
    for (int i = 0; i < n; i++)
      sparse_table[i][0] = a[i];
    for (int j = 1; (1 << j) <= n; j++) {
      for (int i = 0; i + (1 << j) - 1 < n; i++) {
        sparse_table[i][j] = max(sparse_table[i][j - 1],
                                 sparse_table[i + (1 << (j - 1))][j - 1]);
      }
    }
  }

  long long query(int l, int r) {
    if (l > r)
      return LLONG_MIN;
    int len = r - l + 1;
    int p = log2(len);
    return max(sparse_table[l][p], sparse_table[r - (1 << p) + 1][p]);
  }
};

class MaxSparseTable2D {
public:
  int n, m, K;
  std::vector<std::vector<long long>> a;
  std::vector<std::vector<std::vector<long long>>> st;

  MaxSparseTable2D(const std::vector<std::vector<long long>> &matrix) {
    a = matrix;
    n = a.size();
    m = n ? a[0].size() : 0;
    K = std::log2(std::min(n, m)) + 1;
    st.assign(
        n, std::vector<std::vector<long long>>(m, std::vector<long long>(K)));
    build();
  }

  void build() {
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < m; ++j)
        st[i][j][0] = a[i][j];

    for (int k = 1; k < K; ++k) {
      int len = 1 << k, hl = len >> 1;
      for (int i = 0; i + len <= n; ++i) {
        for (int j = 0; j + len <= m; ++j) {
          st[i][j][k] =
              std::max({st[i][j][k - 1], st[i + hl][j][k - 1],
                        st[i][j + hl][k - 1], st[i + hl][j + hl][k - 1]});
        }
      }
    }
  }

  long long query(int x1, int y1, int x2, int y2) {
    if (x1 > x2)
      std::swap(x1, x2);
    if (y1 > y2)
      std::swap(y1, y2);
    int k = std::log2(std::min(x2 - x1 + 1, y2 - y1 + 1));
    int dx = x2 - (1 << k) + 1, dy = y2 - (1 << k) + 1;
    return std::max(
        {st[x1][y1][k], st[dx][y1][k], st[x1][dy][k], st[dx][dy][k]});
  }
};

class min_sparse_table {
public:
  vector<long long> a;
  int n;
  vector<vector<long long>> sparse_table;

  min_sparse_table(vector<long long> &arr) {
    a = arr;
    n = a.size();
    sparse_table.assign(n + 1, vector<long long>(log2(n) + 2));
    build();
  }

  void build() {
    for (int i = 0; i < n; i++)
      sparse_table[i][0] = a[i];
    for (int j = 1; (1 << j) <= n; j++) {
      for (int i = 0; i + (1 << j) - 1 < n; i++) {
        sparse_table[i][j] = min(sparse_table[i][j - 1],
                                 sparse_table[i + (1 << (j - 1))][j - 1]);
      }
    }
  }

  long long query(int l, int r) {
    if (l > r)
      return LLONG_MAX;
    int len = r - l + 1;
    int p = log2(len);
    return min(sparse_table[l][p], sparse_table[r - (1 << p) + 1][p]);
  }
};

class MinSparseTable2D {
public:
  int n, m, K;
  std::vector<std::vector<long long>> a;
  std::vector<std::vector<std::vector<long long>>> st;

  MinSparseTable2D(const std::vector<std::vector<long long>> &matrix) {
    a = matrix;
    n = a.size();
    m = n ? a[0].size() : 0;
    K = std::log2(std::min(n, m)) + 1;
    st.assign(
        n, std::vector<std::vector<long long>>(m, std::vector<long long>(K)));
    build();
  }

  void build() {
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < m; ++j)
        st[i][j][0] = a[i][j];

    for (int k = 1; k < K; ++k) {
      int len = 1 << k, hl = len >> 1;
      for (int i = 0; i + len <= n; ++i) {
        for (int j = 0; j + len <= m; ++j) {
          st[i][j][k] =
              std::min({st[i][j][k - 1], st[i + hl][j][k - 1],
                        st[i][j + hl][k - 1], st[i + hl][j + hl][k - 1]});
        }
      }
    }
  }

  long long query(int x1, int y1, int x2, int y2) {
    if (x1 > x2)
      std::swap(x1, x2);
    if (y1 > y2)
      std::swap(y1, y2);
    int k = std::log2(std::min(x2 - x1 + 1, y2 - y1 + 1));
    int dx = x2 - (1 << k) + 1, dy = y2 - (1 << k) + 1;
    return std::min(
        {st[x1][y1][k], st[dx][y1][k], st[x1][dy][k], st[dx][dy][k]});
  }
};
