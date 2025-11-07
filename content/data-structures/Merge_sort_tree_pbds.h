// Merge Sort Tree with point updates - paste into a .cpp file
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

template <typename T>
using ordered_set = tree<pair<T, int>, null_type, less<pair<T, int>>,
                         rb_tree_tag, tree_order_statistics_node_update>;

class MergeSortTree {
  int n;
  vector<ordered_set<int>> st;
  vector<int> arr;

  void insert_at(int node, int l, int r, int pos, int val) {
    st[node].insert({val, pos});
    if (l == r)
      return;
    int mid = (l + r) >> 1;
    if (pos <= mid)
      insert_at(node << 1, l, mid, pos, val);
    else
      insert_at(node << 1 | 1, mid + 1, r, pos, val);
  }

  void erase_at(int node, int l, int r, int pos, int val) {
    st[node].erase({val, pos});
    if (l == r)
      return;
    int mid = (l + r) >> 1;
    if (pos <= mid)
      erase_at(node << 1, l, mid, pos, val);
    else
      erase_at(node << 1 | 1, mid + 1, r, pos, val);
  }

  int query_le_rec(int node, int l, int r, int ql, int qr, int k) {
    if (qr < l || r < ql)
      return 0;
    if (ql <= l && r <= qr) {
      return (int)st[node].order_of_key({k + 1, numeric_limits<int>::min()});
    }
    int mid = (l + r) >> 1;
    return query_le_rec(node << 1, l, mid, ql, qr, k) +
           query_le_rec(node << 1 | 1, mid + 1, r, ql, qr, k);
  }

public:
  MergeSortTree() : n(0) {}

  MergeSortTree(const vector<int> &a) { build(a); }

  void build(const vector<int> &a) {
    arr = a;
    n = (int)arr.size();
    if (n == 0)
      return;
    st.assign(4 * n, ordered_set<int>());
    for (int i = 0; i < n; ++i)
      insert_at(1, 0, n - 1, i, arr[i]);
  }

  void point_update(int pos, int newVal) {
    if (pos < 0 || pos >= n)
      return;
    int old = arr[pos];
    if (old == newVal)
      return;
    erase_at(1, 0, n - 1, pos, old);
    insert_at(1, 0, n - 1, pos, newVal);
    arr[pos] = newVal;
  }

  int count_le(int l, int r, int k) {
    if (l < 0)
      l = 0;
    if (r >= n)
      r = n - 1;
    if (l > r || n == 0)
      return 0;
    return query_le_rec(1, 0, n - 1, l, r, k);
  }

  int count_lt(int l, int r, int k) { return count_le(l, r, k - 1); }

  int count_gt(int l, int r, int k) { return (r - l + 1) - count_le(l, r, k); }

  const vector<int> &data() const { return arr; }
};

// Example usage:
// int main() {
//     vector<int> a = {5,1,7,3,4};
//     MergeSortTree mst(a);
//     cout << mst.count_le(1,4,3) << "\n";
//     mst.point_update(2,2);
//     cout << mst.count_le(1,4,3) << "\n";
// }
