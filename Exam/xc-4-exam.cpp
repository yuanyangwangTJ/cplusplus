#include <algorithm>
#include <iostream>
#include <numeric>
#include <stack>
#include <vector>
#include <bitset>

using namespace std;

using bit = bitset<1>;

// 定义树的结构
struct TreeNode {
    int data;
    int parent_idx;
    vector<int> child_idxs;
};

class Tree {
public:
    vector<TreeNode> nodes_;
    int node_num_ = 0;

public:
    auto get(int i) {
        return nodes_[i];
    }
    auto getNum() { return node_num_; }
    auto AddNode(int data) {
        nodes_.emplace_back(TreeNode{data, -1});
        node_num_++;
    }
    auto BuildTree(int parent, int child) {
        nodes_[child].parent_idx = parent;
        nodes_[parent].child_idxs.push_back(child);
    }
    auto CalSum() {
        return accumulate(nodes_.begin(), nodes_.end(), 0, 
            [&](int sum, TreeNode& node){
                return sum + node.data;
            });
    }
};

auto InputTree() {
    Tree tree;
    int num;
    cin >> num;
    for (int i = 0; i < num; ++i) {
        int data;
        cin >> data;
        tree.AddNode(data);
    }

    for (int i = 0; i < num-1; ++i) {
        int parent, child;
        cin >> parent >> child;
        tree.BuildTree(parent-1, child-1);
    }
    return tree;
}

auto levelOrderOnelayer(const Tree& tree, const vector<int>& node_idxs, stack<int>& s) {
    if (node_idxs.empty()) {
        return;
    }

    vector<int> next_idxs;
    for (auto i : node_idxs) {
        s.push(i);
        for (auto e : tree.nodes_[i].child_idxs) {
            next_idxs.push_back(e);
        }
    }
    levelOrderOnelayer(tree, next_idxs, s);
}

auto LevelOrder(const Tree& tree) {
    stack<int> s;
    auto& nodes = tree.nodes_;
    auto root_it = find_if(nodes.begin(), nodes.end(), [&](const TreeNode& node) {
        return node.parent_idx == -1;
    });

    levelOrderOnelayer(tree, {int(distance(nodes.begin(), root_it))}, s);
    return s;
}

auto Convert(Tree& tree, vector<pair<int, int>>& ops) {
    vector<bit> node_stats;
    ops.clear();
    auto& nodes = tree.nodes_;
    
    for_each(nodes.begin(), nodes.end(),[&](TreeNode& node) {
        node_stats.push_back(node.data % 2 == 0 ? 0 : 1);
    });

    bit stat = 1;
    if (tree.CalSum() % 2 == 0) {
        stat = 0;
    }

    // 从子节点开始处理
    auto stack = LevelOrder(tree);
    while (!stack.empty()) {
        int idx = stack.top();
        stack.pop();
        if (node_stats[idx] != stat) {
            // 变换
            node_stats[idx] = ~node_stats[idx];
            int parent = nodes[idx].parent_idx;
            if (parent == -1) {
                // 根节点，无法达成目标
                return false;
            }
            node_stats[parent] = ~node_stats[parent];
            ops.push_back({parent, idx});
        }
    }
    return true;
}


int main() {
    auto tree = InputTree();
    int sum = tree.CalSum();
    bit status = 1;     // 记录奇偶
    if (sum % 2 == 0) {
        status = 0;
    }
    cout << sum << endl;
    auto s = LevelOrder(tree);
    auto tmp = s;
    vector<pair<int, int>> ops;
    if (Convert(tree, ops)) {
        cout << "Yes" << endl;
        for (const auto& op : ops) {
            cout << op.first + 1 << " " << op.second + 1 << endl;
        }
    } else {
        cout << "No" << endl;
    }

    return 0;
}
