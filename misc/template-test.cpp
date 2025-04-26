#include <iostream>

// using namespace std;

struct Node
{
    int value;
    Node* left;
    Node* right;
    Node(int i = 0) : value(i), left(nullptr), right(nullptr) {}
};


// traverse tree, using fold expression
template<typename T, typename... TP>
Node* traverse(T np, TP... paths) {
    return (np ->* ... ->* paths);
}

auto left = &Node::left;
auto right = &Node::right;

int main()
{
    Node* root = new Node{0};
    root->left = new Node{1};
    root->left->right = new Node{2};

    Node* node = traverse(root, left, right);

    return 0;
}
