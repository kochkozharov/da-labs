#include <iostream>
#include <vector>

class PersistentSegmentTree {
private:
    int cnt = 0;
    struct Node {
        int value;
        Node* left;
        Node* right;
        Node(int v = 0) : value(v), left(nullptr), right(nullptr) {}
    };

    std::vector<Node*> roots;
    int size;

    Node* build(int start, int end) {
        Node* node = new Node();
        if (start == end) {
            return node;
        }
        int mid = (start + end) / 2;
        node->left = build(start, mid);
        node->right = build(mid + 1, end);
        return node;
    }

    int query(Node* node, int start, int end, int l, int r) {
        if (!node || r < start || end < l) {
            return 0;
        }
        if (l <= start && end <= r) {
            return node->value;
        }
        int mid = (start + end) / 2;
        return query(node->left, start, mid, l, r) + query(node->right, mid + 1, end, l, r);
    }

    Node* update(Node* node, int start, int end, int ind, int val) {
        Node* newNode = new Node(*node);
        if (start == end) {
            newNode->value += val;
        } else {
            int mid = (start + end) / 2;
            if (ind <= mid) {
                newNode->left = update(node->left, start, mid, ind, val);
            } else {
                newNode->right = update(node->right, mid + 1, end, ind, val);
            }
            newNode->value = newNode->left->value + newNode->right->value;
        }
        return newNode;
    }


    void printTree(Node* node, int level = 0) {
        if (!node) {
            return;
        }
        printTree(node->right, level + 1);
        std::cout << std::string(level * 4, ' ') << node->value << std::endl;
        printTree(node->left, level + 1);
    }


public:
    PersistentSegmentTree(int size) : size(size) {
        roots.resize(size);
        roots[0] = build(0, size - 1);
    }

    int query(int x, int l, int r) {
        return query(roots[x], 0, size - 1, l, r);
    }

    void update(int x, int ind, int val) {
        cnt++;
        roots[cnt] = update(roots[cnt - 1], 0, size - 1, ind, val);
    }
};

