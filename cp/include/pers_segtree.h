#include <iostream>
#include <vector>

class PersistentSegmentTree {
private:
    int cnt = 0;
    struct Node {
        int value;
        Node* left;
        Node* right;
        Node(int v, int start, int end) : value(v), left(nullptr), right(nullptr) {
            if (start == end) {
                return;
            }
            int mid = (start + end) / 2;
            left = new Node(0, start, mid);
            right = new Node(0, mid + 1, end);
        }
        int query(int start, int end, int l, int r) {
            if (r < start || end < l) {
                return 0;
            }
            if (l <= start && end <= r) {
                return value;
            }
            int mid = (start + end) / 2;
            return left->query( start, mid, l, r) + right->query(mid + 1, end, l, r);
        }
        void update(int start, int end, int ind, int val) {
            if (left) {
                left = new Node(*left);
                right = new Node(*right);
            }
            if (start == end) {
                value += val;
            } else {
                int mid = (start + end) / 2;
                if (ind <= mid) {
                    left->update(start, mid, ind, val);
                } else {
                    right->update(mid + 1, end, ind, val);
                }
                value = left->value + right->value;
            }
        }
    };


    std::vector<Node*> roots;
    int size;

public:
    PersistentSegmentTree(int size) : size(size) {
        roots.resize(size);
        roots[0] = new Node(0, 0, size - 1);
    }

    int query(int x, int l, int r) {
        return roots[x]->query( 0, size - 1, l, r);
    }

    void update(int x, int ind, int val) {
        cnt++;
        auto newRoot = new Node(*roots[cnt-1]);
        newRoot->update(0, size - 1, ind, val);
        roots[cnt] = newRoot;
    }
};

