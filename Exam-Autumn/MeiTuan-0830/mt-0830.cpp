#include <iostream>
#include <numeric>
#include <vector>
#include <queue>
#include <cstdint>
#include <climits>
#include <unordered_map>
#include <algorithm>

using namespace std;

enum class COLOR {
    B = 0,
    W = 1
};

struct Node {
    int w;
    COLOR c;
    int parent = -1;
};

using Tree = vector<Node>;
const int64_t MOD = 1000000007;

void ReverseNode(Tree& tree, int node) {
    tree[node].c = static_cast<COLOR>(1 - static_cast<int>(tree[node].c));
}

// 试除分解，返回按质因数升序的 (prime, exp)
static inline vector<pair<long long,int>> factorize(long long n) {
    vector<pair<long long,int>> f;
    if (n <= 1) return f;
    for (long long p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            f.push_back({p, e});
        }
    }
    if (n > 1) f.push_back({n, 1});
    sort(f.begin(), f.end());
    return f;
}

static inline long long mod_pow(long long a, long long e) {
    long long r = 1 % MOD;
    a %= MOD;
    while (e > 0) {
        if (e & 1) r = (__int128)r * a % MOD;
        a = (__int128)a * a % MOD;
        e >>= 1;
    }
    return r;
}

// 合并两个按 prime 升序的向量，指数取最大值
static inline vector<pair<long long,int>> mergeMax(const vector<pair<long long,int>>& A,
                                                   const vector<pair<long long,int>>& B) {
    vector<pair<long long,int>> C;
    C.reserve(A.size() + B.size());
    size_t i = 0, j = 0;
    while (i < A.size() && j < B.size()) {
        if (A[i].first == B[j].first) {
            C.push_back({A[i].first, max(A[i].second, B[j].second)});
            ++i; ++j;
        } else if (A[i].first < B[j].first) {
            C.push_back(A[i++]);
        } else {
            C.push_back(B[j++]);
        }
    }
    while (i < A.size()) C.push_back(A[i++]);
    while (j < B.size()) C.push_back(B[j++]);
    return C;
}

// 线段树维护 factor 向量（prime 升序，指数为该段最大值）
struct SegTree {
    int n;
    vector<vector<pair<long long,int>>> seg;

    SegTree() : n(0) {}
    explicit SegTree(int n_) { init(n_); }

    void init(int n_) {
        n = n_;
        seg.assign(4 * n + 4, {});
    }

    void build(const vector<vector<pair<long long,int>>>& base, int idx, int l, int r) {
        if (l == r) {
            seg[idx] = base[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(base, idx << 1, l, mid);
        build(base, idx << 1 | 1, mid + 1, r);
        seg[idx] = mergeMax(seg[idx << 1], seg[idx << 1 | 1]);
    }

    void build(const vector<vector<pair<long long,int>>>& base) {
        build(base, 1, 1, n);
    }

    void update(int pos, const vector<pair<long long,int>>& val, int idx, int l, int r) {
        if (l == r) { seg[idx] = val; return; }
        int mid = (l + r) >> 1;
        if (pos <= mid) update(pos, val, idx << 1, l, mid);
        else update(pos, val, idx << 1 | 1, mid + 1, r);
        seg[idx] = mergeMax(seg[idx << 1], seg[idx << 1 | 1]);
    }

    void update(int pos, const vector<pair<long long,int>>& val) { update(pos, val, 1, 1, n); }

    vector<pair<long long,int>> query(int ql, int qr, int idx, int l, int r) {
        if (ql <= l && r <= qr) return seg[idx];
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(ql, qr, idx << 1, l, mid);
        if (ql > mid) return query(ql, qr, idx << 1 | 1, mid + 1, r);
        auto L = query(ql, qr, idx << 1, l, mid);
        auto R = query(ql, qr, idx << 1 | 1, mid + 1, r);
        return mergeMax(L, R);
    }

    vector<pair<long long,int>> query(int l, int r) { return query(l, r, 1, 1, n); }
};

// Heavy-Light Decomposition
static int n_global;
static vector<vector<int>> adj;
static vector<int> parent_arr, depth_arr, heavy, head, pos, sz;
static int cur_pos = 0;

int dfs1(int u, int p) {
    parent_arr[u] = p;
    depth_arr[u] = (p == -1 ? 0 : depth_arr[p] + 1);
    sz[u] = 1;
    int max_sz = 0;
    heavy[u] = -1;
    for (int v : adj[u]) if (v != p) {
        int sub = dfs1(v, u);
        sz[u] += sub;
        if (sub > max_sz) { max_sz = sub; heavy[u] = v; }
    }
    return sz[u];
}

void dfs2(int u, int h) {
    head[u] = h;
    pos[u] = ++cur_pos;
    if (heavy[u] != -1) {
        dfs2(heavy[u], h);
        for (int v : adj[u]) if (v != parent_arr[u] && v != heavy[u]) dfs2(v, v);
    }
}

long long evalProductMod(const vector<pair<long long,int>>& fv) {
    long long ans = 1;
    for (const auto& pe : fv) {
        ans = (__int128)ans * mod_pow(pe.first, pe.second) % MOD;
    }
    return ans;
}

int main() {
    int n, q;
    cin >> n >> q;
    n_global = n;
    Tree tree(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> tree[i].w;
    }
    vector<vector<pair<long long,int>>> factors(n + 1);
    for (int i = 1; i <= n; ++i) factors[i] = factorize(tree[i].w);
    string s;
    cin >> s;
    for (int i = 1; i <= n; ++i) {
        char ch = s[i - 1];
        if (ch == 'W') tree[i].c = COLOR::W; else tree[i].c = COLOR::B;
    }

    adj.assign(n + 1, {});
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // HLD 准备
    parent_arr.assign(n + 1, -1);
    depth_arr.assign(n + 1, 0);
    heavy.assign(n + 1, -1);
    head.assign(n + 1, -1);
    pos.assign(n + 1, 0);
    sz.assign(n + 1, 0);
    cur_pos = 0;

    int root = 1; // 若题目指定根，请替换
    dfs1(root, -1);
    dfs2(root, root);

    // 基数组：按 dfn 位置放置叶子值（B 节点放因子，W 放空）
    vector<vector<pair<long long,int>>> base(n + 1);
    for (int u = 1; u <= n; ++u) {
        base[pos[u]] = (tree[u].c == COLOR::B) ? factors[u] : vector<pair<long long,int>>{};
    }

    // 段树
    SegTree st(n);
    st.init(n);
    st.build(base);

    auto query_to_root = [&](int u) {
        vector<pair<long long,int>> acc; // 空表示 1
        while (head[u] != head[root]) {
            int hu = head[u];
            auto part = st.query(pos[hu], pos[u]);
            acc = mergeMax(acc, part);
            u = parent_arr[hu];
        }
        auto part = st.query(pos[root], pos[u]);
        acc = mergeMax(acc, part);
        return evalProductMod(acc);
    };

    while (q--) {
        int t, x;
        cin >> t >> x;
        if (t == 1) {
            ReverseNode(tree, x);
            // 点更新
            if (tree[x].c == COLOR::B) st.update(pos[x], factors[x]);
            else st.update(pos[x], {});
        } else {
            auto ans = query_to_root(x);
            cout << ans << '\n';
        }
    }

    return 0;
}