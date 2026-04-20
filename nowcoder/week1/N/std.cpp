#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    Node *l, *r;
    Node(int v) : val(v), l(nullptr), r(nullptr) {}
};

// 将值 v 插入以 root 为根的 BST，返回插入节点的父节点（根插入返回 nullptr）
Node* insertBST(Node*& root, int v) {
    if (root == nullptr) {
        root = new Node(v);
        return nullptr;
    }
    Node* cur = root;
    Node* parent = nullptr;
    while (cur != nullptr) {
        parent = cur;
        if (v < cur->val) cur = cur->l;
        else cur = cur->r;
    }
    Node* node = new Node(v);
    if (v < parent->val) parent->l = node;
    else parent->r = node;
    return parent;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    Node* root = nullptr;
    for (int i = 0; i < n; ++i) {
        int v;
        cin >> v;
        Node* parent = insertBST(root, v);
        if (parent == nullptr) cout << -1 << '\n';
        else cout << parent->val << '\n';
    }

    return 0;
}
