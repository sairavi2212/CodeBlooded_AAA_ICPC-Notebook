struct node {};

node merge_node(const node &a, const node &b) {
  // TODO: implement merging logic for your problem
  return node();
}

struct SegTree {
  int n;
  int off;
  vector<node> st;
  SegTree(const vector<node> &init) {
    n = (int)init.size();
    off = 1;
    while (off < n)
      off <<= 1;
    st.assign(2 * off, node());
    for (int i = 0; i < n; ++i)
      st[off + i] = init[i];
    for (int i = n; i < off; ++i)
      st[off + i] = node(); // empty
    for (int i = off - 1; i >= 1; --i)
      st[i] = merge_node(st[2 * i], st[2 * i + 1]);
  }
  void point_set(int pos, const node &val) {
    int p = off + pos;
    st[p] = val;
    for (p >>= 1; p >= 1; p >>= 1)
      st[p] = merge_node(st[2 * p], st[2 * p + 1]);
  }
  node range_query(int left, int r) {
    if (left > r)
      return node();
    node left_id, right_id;
    int L = left + off, R = r + off;
    while (L <= R) {
      if (L & 1)
        left_id = merge_node(left_id, st[L++]);
      if (!(R & 1))
        right_id = merge_node(st[R--], right_id);
      L >>= 1;
      R >>= 1;
    }
    return merge_node(left_id, right_id);
  }
};

// USAGE NOTES:
// - Implement `node` fields and `merge_node` according to your operation (sum,
// min, max, gcd, custom struct, etc.).
// - If you need identity elements, ensure default `node()` acts as identity or
// replace left_id/right_id initializers.
// - Add #include <bits/stdc++.h> and `using namespace std;` in file if needed.
