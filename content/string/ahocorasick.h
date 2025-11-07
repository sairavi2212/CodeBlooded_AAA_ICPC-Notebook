class AhoCorasick {
public:
    static const int K = 26;

    struct Vertex {
        int next[K];
        int p = -1;
        char pch;
        int link = -1;
        int go[K];
        vector<int> out; // pattern indices that end here

        Vertex(int p = -1, char ch = '$') : p(p), pch(ch) {
            fill(begin(next), end(next), -1);
            fill(begin(go), end(go), -1);
        }
    };

    vector<Vertex> t;
    int pattern_count = 0; // next pattern id to assign

    AhoCorasick() {
        t.emplace_back(); // root vertex (index 0)
    }

    // add a lowercase 'a'..'z' pattern, returns its pattern index (0-based)
    int add_string(const string &s) {
        int v = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (c < 0 || c >= K) throw runtime_error("patterns must be lowercase a-z");
            if (t[v].next[c] == -1) {
                t[v].next[c] = (int)t.size();
                t.emplace_back(v, ch);
            }
            v = t[v].next[c];
        }
        int id = pattern_count++;
        t[v].out.push_back(id);
        return id;
    }

    // returns vector of (text_index, patternIndex) pairs for every match
    vector<pair<int,int>> search(const string &text) {
        vector<pair<int,int>> res;
        int v = 0;
        for (int i = 0; i < (int)text.size(); ++i) {
            char ch = text[i];
            if (ch < 'a' || ch > 'z') {
                // reset on invalid char (same behaviour as before)
                v = 0;
                continue;
            }
            v = go(v, ch);

            // follow output/link chain to collect matches
            for (int u = v; u != 0; u = get_link(u)) {
                for (int patId : t[u].out) {
                    res.emplace_back(i, patId);
                }
            }
            // optionally check root outputs (only relevant if you inserted empty pattern)
            for (int patId : t[0].out) {
                res.emplace_back(i, patId);
            }
        }
        return res;
    }

    int get_link(int v) {
        if (t[v].link == -1) {
            if (v == 0 || t[v].p == 0)
                t[v].link = 0;
            else
                t[v].link = go(get_link(t[v].p), t[v].pch);
        }
        return t[v].link;
    }

    int go(int v, char ch) {
        int c = ch - 'a';
        if (t[v].go[c] == -1) {
            if (t[v].next[c] != -1)
                t[v].go[c] = t[v].next[c];
            else
                t[v].go[c] = v == 0 ? 0 : go(get_link(v), ch);
        }
        return t[v].go[c];
    }
};