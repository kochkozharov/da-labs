#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Trie {
private:
    struct Vertex {
        int l {-1};
        int r {-1};
        int enter {-1};
        map<char, shared_ptr<Vertex>> child;
    };

    shared_ptr<Vertex> root;
    string text;

    void Insert(int l, int r) {
        auto current = root;
        int oldL = l;

        while (l <= r) {
            if (current == root) {
                if (current->child.find(text[l]) != current->child.end()) {
                    current = current->child[text[l]];
                } else {
                    current->child[text[l]] = NewVertex(l, r, oldL);
                    break;
                }
            }

            int start = current->l;
            int finish = current->r;
            bool cut = false;

            for (int i = start; (i <= finish) && (l + i - start <= r); ++i) {
                if (text[i] != text[l + i - start]) {
                    current->r = i - 1;
                    int oldEnter = current->enter;
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
                int newL = l + finish - start + 1;
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

    static shared_ptr<Vertex> NewVertex(int l, int r) {
        auto vertex = make_shared<Vertex>();
        vertex->l = l;
        vertex->r = r;
        return vertex;
    }

    static shared_ptr<Vertex> NewVertex(int l, int r, int enter) {
        auto vertex = make_shared<Vertex>();
        vertex->l = l;
        vertex->r = r;
        vertex->enter = enter;
        return vertex;
    }

    void DFS(vector<int> &res, shared_ptr<Vertex> current) {
        if (current->child.empty()) {
            res.push_back(current->enter);
        }
        for (const auto &[first, child] : current->child) {
            DFS(res, child);
        }
    }

public:
    // Конструктор для создания дерева и построения суффиксного дерева по тексту
    Trie(const string &inputText) {
        text = inputText + '$';
        root = make_shared<Vertex>();
        for (size_t i = 0; i < text.length(); ++i)
            Insert(i, text.length() - 1);
    }

    vector<int> Search(const string &pattern) {
        vector<int> res;
        auto current = root;
        int l = 0;
        int r = pattern.length() - 1;
        bool flag = false;

        while (l <= r) {
            if (current == root) {
                if (current->child.find(pattern[l]) != current->child.end()) {
                    current = current->child[pattern[l]];
                } else {
                    break;
                }
            }

            int start = current->l;
            int finish = current->r;
            for (int i = 0; (start + i <= finish) && (i + l <= r); ++i) {
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
};

int main() {
    string text;
    cin >> text;
    Trie trie(text);

    string pattern;
    int counter = 1;
    while (cin >> pattern) {
        vector<int> res = trie.Search(pattern);
        if (!res.empty()) {
            printf("%d: ", counter);
            sort(res.begin(), res.end());
            for (size_t i = 0; i < res.size(); ++i) {
                if (i != 0) {
                    printf(", ");
                }
                printf("%d", res[i] + 1);
            }
            printf("\n");
        }
        ++counter;
    }

    return 0;
}
