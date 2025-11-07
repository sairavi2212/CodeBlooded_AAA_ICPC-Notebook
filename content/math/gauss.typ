== gauss.h
```cpp
#ifndef ICPC_SLAE_H
#define ICPC_SLAE_H

#include <algorithm>
#include <climits>
#include <cmath>
#include <vector>

namespace cp_algo {

using Real = double;
constexpr Real EPS = 1e-9;
constexpr int INF_SOLS = INT_MAX;

int solve_slae(std::vector<std::vector<Real>> &a, std::vector<Real> &ans) {
  if (a.empty())
    return 0;
  int n = (int)a.size();
  int m = (int)a[0].size() - 1;

  std::vector<int> where(m, -1);
  int row = 0;
  for (int col = 0; col < m && row < n; ++col) {
    int sel = row;
    for (int i = row; i < n; ++i) {
      if (std::abs(a[i][col]) > std::abs(a[sel][col]))
        sel = i;
    }

    if (std::abs(a[sel][col]) < EPS)
      continue;

    std::swap(a[sel], a[row]);
    where[col] = row;

    Real div = a[row][col];
    for (int j = col; j <= m; ++j)
      a[row][j] /= div;

    for (int i = 0; i < n; ++i) {
      if (i != row) {
        Real mult = a[i][col];
        for (int j = col; j <= m; ++j)
          a[i][j] -= mult * a[row][j];
      }
    }
    ++row;
  }

  ans.assign(m, 0);
  for (int j = 0; j < m; ++j) {
    if (where[j] != -1)
      ans[j] = a[where[j]][m];
  }

  for (int i = 0; i < n; ++i) {
    Real sum = 0;
    for (int j = 0; j < m; ++j)
      sum += ans[j] * a[i][j];
    if (std::abs(sum - a[i][m]) > EPS)
      return 0;
  }

  for (int j = 0; j < m; ++j) {
    if (where[j] == -1)
      return INF_SOLS;
  }

  return 1;
}
} // namespace cp_algo

#endif
```