struct BlockCutTree {
  int n;
  vector<vector<int>> adj;
  vector<int> tin, low;
  int timer;
  stack<int> st;
  vector<vector<int>> tree_adj;
  vector<int> is_ap;
  int num_blocks;

  BlockCutTree(int n)
      : n(n), adj(n), tin(n, -1), low(n, -1), timer(0), is_ap(n, 0),
        num_blocks(0) {}

  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  void dfs(int u, int p = -1) {
    tin[u] = low[u] = timer++;
    st.push(u);
    int children = 0;
    for (int v : adj[u]) {
      if (v == p)
        continue;
      if (tin[v] != -1) {
        low[u] = min(low[u], tin[v]);
      } else {
        dfs(v, u);
        low[u] = min(low[u], low[v]);
        if (low[v] >= tin[u] && p != -1)
          is_ap[u] = 1;
        children++;
        if (low[v] >= tin[u]) {
          int block_node = n + num_blocks++;
          tree_adj.emplace_back();
          tree_adj[block_node].push_back(u);
          tree_adj[u].push_back(block_node);
          while (true) {
            int node = st.top();
            st.pop();
            if (node != u) {
              tree_adj[block_node].push_back(node);
              tree_adj[node].push_back(block_node);
            }
            if (node == v)
              break;
          }
        }
      }
    }
    if (p == -1 && children > 1)
      is_ap[u] = 1;
  }

  void build() {
    timer = num_blocks = 0;
    tree_adj.assign(n, {});
    fill(tin.begin(), tin.end(), -1);
    fill(is_ap.begin(), is_ap.end(), 0);
    while (!st.empty())
      st.pop();
    for (int i = 0; i < n; ++i) {
      if (tin[i] == -1) {
        dfs(i);
        while (!st.empty())
          st.pop();
      }
    }
  }

  int size() const { return n + num_blocks; }
};