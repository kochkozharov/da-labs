#include <memory>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class Trie {
private:
    struct Vertex {
        size_t l;
        size_t r;
        size_t enter;
        std::map<char, std::shared_ptr<Vertex>> child;
    };

    std::shared_ptr<Vertex> root;
    std::string text;

    void Insert(size_t l, size_t r);
    static std::shared_ptr<Vertex> NewVertex(size_t l, size_t r, size_t enter);
    void DFS(std::vector<size_t>& res, std::shared_ptr<Vertex> current);

public:
    explicit Trie(const std::string& inputText);
    std::vector<size_t> Search(const std::string& pattern);
};


void Trie::Insert(size_t l, size_t r) {
    auto current = root;
    size_t oldL = l;

    while (l <= r) {
        if (current == root) {
            if (current->child.find(text[l]) != current->child.end()) {
                current = current->child[text[l]];
            } else {
                current->child[text[l]] = NewVertex(l, r, oldL);
                break;
            }
        }

        size_t start = current->l;
        size_t finish = current->r;
        bool cut = false;

        for (size_t i = start; (i <= finish) && (l + i - start <= r); ++i) {
            if (text[i] != text[l + i - start]) {
                current->r = i - 1;
                size_t oldEnter = current->enter;
                current->enter = -1;

                auto splitNode = NewVertex(i, finish, oldEnter);
                splitNode->child = move(current->child);
                current->child.clear();
                current->child[text[i]] = splitNode;

                current->child[text[l + i - start]] = NewVertex(l + i - start, r, oldL);
                cut = true;
                break;
            }
        }

        if (!cut) {
            size_t newL = l + finish - start + 1;
            if (current->child.find(text[newL]) != current->child.end()) {
                current = current->child[text[newL]];
            } else {
                current->child[text[newL]] = NewVertex(newL, r, oldL);
                break;
            }
            l = newL;
        } else {
            break;
        }
    }
}


std::shared_ptr<Trie::Vertex> Trie::NewVertex(size_t l, size_t r, size_t enter) {
    auto vertex = std::make_shared<Vertex>();
    vertex->l = l;
    vertex->r = r;
    vertex->enter = enter;
    return vertex;
}

void Trie::DFS(std::vector<size_t> &res, std::shared_ptr<Trie::Vertex> current) {
    if (current->child.empty()) {
        res.push_back(current->enter);
    }
    for (const auto &[first, child] : current->child) {
        DFS(res, child);
    }
}

Trie::Trie(const std::string &inputText) {
    text = inputText + '$';
    root = std::make_shared<Vertex>();
    for (size_t i = 0; i < text.length(); ++i)
        Insert(i, text.length() - 1);
}

std::vector<size_t> Trie::Search(const std::string &pattern) {
    std::vector<size_t> res;
    auto current = root;
    size_t l = 0;
    size_t r = pattern.length() - 1;
    bool flag = false;

    while (l <= r) {
        if (current == root) {
            if (current->child.find(pattern[l]) != current->child.end()) {
                current = current->child[pattern[l]];
            } else {
                break;
            }
        }

        size_t start = current->l;
        size_t finish = current->r;
        for (size_t i = 0; (start + i <= finish) && (i + l <= r); ++i) {
            if (pattern[i + l] != text[start + i]) {
                flag = true;
                break;
            }
        }

        if (!flag) {
            l = l + finish - start + 1;
            if (l > r) {
                break;
            }
            if (current->child.find(pattern[l]) != current->child.end()) {
                current = current->child[pattern[l]];
            } else {
                break;
            }
        } else {
            break;
        }
    }

    if ((l > r) && (!flag) && (!pattern.empty())) {
        DFS(res, current);
    }
    return res;
}

int main() {
    string text;
    cin >> text;
    Trie trie(text);

    string pattern;
    size_t counter = 1;
    while (cin >> pattern) {
        vector<size_t> res = trie.Search(pattern);
        if (!res.empty()) {
            printf("%zu: ", counter);
            sort(res.begin(), res.end());
            for (size_t i = 0; i < res.size(); ++i) {
                if (i != 0) {
                    printf(", ");
                }
                printf("%zu", res[i] + 1);
            }
            printf("\n");
        }
        ++counter;
    }

    return 0;
}
