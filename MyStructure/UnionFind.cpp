#include <iostream>
#include <numeric>

class UnionFind {
public:
    // n 表示集合中的元素个数，元素按照 0 - n-1编号
    UnionFind(int n) : comp_count(n) {
        if (n < 0) {
            throw std::invalid_argument("n should not less than 0");
        }
        parent.resize(n);
        std::iota(parent.begin(), parent.end(), 0);
        rank.assign(n, 0);
    }

    int Find(int x) {
        // return parent[x] == x ? x : Find(parent[x]);
        // 路径压缩递归写法
        // if (parent[x] != x) {
        //     parent[x] = Find(parent[x]);
        // }
        // return parent[x];
        
        int root = x;
        while (root != parent[root]) root = parent[root];
        // 使用路径压缩
        while (x != root) {
            int nxt = parent[x];
            parent[x] = root;
            x = nxt;
        }
        return root;
    }

    void Union(int x, int y) {
        int rx = Find(x);
        int ry = Find(y);
        if (rx == ry) return;
        // 将低rank的根合并到高rank
        if (rank[rx] < rank[ry]) {
            parent[rx] = ry;
        } else if (rank[rx] > rank[ry]) {
            parent[ry] = rx;
        } else {
            parent[rx] = ry;
            ++rank[rx];
        }
        --comp_count;
    }

    bool Connected(int x, int y) {
        return Find(x) == Find(y);
    }

    int count() const {
        return comp_count;
    }

private:
    std::vector<int> parent;
    std::vector<int> rank;
    int comp_count;     // 连通分数量
};

int main()
{
    UnionFind uf(6);
    uf.Union(0,1);
    uf.Union(1,2);
    uf.Union(3,4);
    std::cout << uf.Connected(0, 2) << "\n"; // 1
    std::cout << uf.Connected(0, 3) << "\n"; // 0
    std::cout << uf.count() << "\n"; // 3 (groups: {0,1,2}, {3,4}, {5})
}