struct DSU {
    vector<int> p;
    DSU(int n) { p.resize(n); iota(p.begin(), p.end(), 0); }
    int find(int i) { return p[i] == i ? i : p[i] = find(p[i]); }
    void unite(int i, int j) { int r1 = find(i), r2 = find(j); if (r1 != r2) p[r1] = r2; }
};

struct BridgeTree {
    int n, timer, tree_n;
    vector<vector<int>> adj, tree_adj;
    vector<int> tin, low, comp;
    vector<pair<int, int>> bridges;
    BridgeTree(int n) : n(n), adj(n), tin(n), low(n), comp(n), tree_n(0) {}
    void add_edge(int u, int v) { adj[u].push_back(v); adj[v].push_back(u); }
    void dfs(int u, int p = -1) {
        tin[u] = low[u] = timer++;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (tin[v] != -1) low[u] = min(low[u], tin[v]);
            else {
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) bridges.push_back({u, v});
            }
        }
    }
    void build() {
        timer = 0;
        fill(tin.begin(), tin.end(), -1);
        fill(low.begin(), low.end(), -1);
        bridges.clear();
        for (int i = 0; i < n; ++i) if (tin[i] == -1) dfs(i);
        DSU dsu(n);
        for (int u = 0; u < n; ++u) {
            for (int v : adj[u]) {
                if (low[v] <= tin[u] && low[u] <= tin[v]) dsu.unite(u, v);
            }
        }
        tree_n = 0;
        comp.assign(n, -1);
        vector<int> r2c(n, -1);
        for (int i = 0; i < n; ++i) if (dsu.find(i) == i) r2c[i] = tree_n++;
        for (int i = 0; i < n; ++i) comp[i] = r2c[dsu.find(i)];
        tree_adj.assign(tree_n, {});
        for (auto& e : bridges) {
            int u = comp[e.first], v = comp[e.second];
            tree_adj[u].push_back(v); tree_adj[v].push_back(u);
        }
    }
};

struct DynamicBridgeTree {
    BridgeTree bt;
    DSU dsu;
    vector<int> par;
    DynamicBridgeTree(int n) : bt(n), dsu(0) {}
    void init(const vector<pair<int,int>>& edges) {
        for(auto& e : edges) bt.add_edge(e.first, e.second);
        bt.build();
        dsu = DSU(bt.tree_n);
        par.assign(bt.tree_n, -1);
        vector<int> q; vector<bool> vis(bt.tree_n, 0);
        for(int i=0; i<bt.tree_n; ++i) {
            if(vis[i]) continue;
            q.push_back(i); vis[i] = 1;
            int h = 0;
            while(h < (int)q.size()){
                int u = q[h++];
                for(int v : bt.tree_adj[u]) if(!vis[v]) { vis[v]=1; par[v]=u; q.push_back(v); }
            }
        }
    }
    void add(int u, int v) {
        u = dsu.find(bt.comp[u]); v = dsu.find(bt.comp[v]);
        if(u == v) return;
        vector<int> pu, pv;
        for(int c=u; c!=-1; c=(par[c]==-1?-1:dsu.find(par[c]))) pu.push_back(c);
        for(int c=v; c!=-1; c=(par[c]==-1?-1:dsu.find(par[c]))) pv.push_back(c);
        reverse(pu.begin(), pu.end()); reverse(pv.begin(), pv.end());
        int lca = -1;
        for(size_t i=0; i<min(pu.size(), pv.size()); ++i) if(pu[i]==pv[i]) lca=pu[i]; else break;
        if(lca == -1) return;
        for(int n : {u, v}) while((n=dsu.find(n)) != lca) { dsu.unite(n, lca); }
    }
};
