#pragma once
#include <vector>
#include <memory>


struct Segment {
    int x, height, type;
    Segment(int x, int height, int type) : x(x), height(height), type(type) {}
};

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

class PersistentSegmentTree {
private:
    struct Node {
        int value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(int value = 0) : value(value), left(nullptr), right(nullptr) {}
    };

    std::vector<std::shared_ptr<Node>> roots;
    int size;

    std::shared_ptr<Node> Build(int start, int end) {
        auto node = std::make_shared<Node>();
        if (start == end) {
            return node;
        }
        int mid = (start + end) / 2;
        node->left = Build(start, mid);
        node->right = Build(mid + 1, end);
        return node;
    }

    int Sum(const std::shared_ptr<Node>& node, int start, int end, int left, int right) const {
        if (!node || right < start || end < left) {
            return 0;
        }
        if (left <= start && end <= right) {
            return node->value;
        }
        int mid = (start + end) / 2;
        return Sum(node->left, start, mid, left, right) + Sum(node->right, mid + 1, end, left, right);
    }

    std::shared_ptr<Node> Set(const std::shared_ptr<Node>& node, int start, int end, int index, int value) {
        auto newNode = std::make_shared<Node>(*node);
        if (start == end) {
            newNode->value += value;
        } else {
            int mid = (start + end) / 2;
            if (index <= mid) {
                newNode->left = Set(node->left, start, mid, index, value);
            } else {
                newNode->right = Set(node->right, mid + 1, end, index, value);
            }
            newNode->value = (newNode->left ? newNode->left->value : 0) + 
                             (newNode->right ? newNode->right->value : 0);
        }
        return newNode;
    }

public:
    PersistentSegmentTree(int size) : size(size) {
        roots.push_back(Build(0, size - 1));
    }

    void Set(int version, int index, int heightIndex, int value) {
        roots.push_back(Set(roots[version], 0, size - 1, heightIndex, value));
    }

    int Sum(int version, int left, int right) const {
        return Sum(roots[version], 0, size - 1, left, right);
    }

    int Versions() const {
        return roots.size();
    }
};