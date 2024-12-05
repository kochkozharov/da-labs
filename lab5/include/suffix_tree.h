#pragma once 

#include <memory>
#include <map>
#include <vector>
#include <string>

class SuffixTrie {
private:
    struct Node {
        size_t l;
        size_t r;
        size_t enter;
        std::map<char, std::shared_ptr<Node>> child;
    };

    std::shared_ptr<Node> root;
    std::string text;

    void Insert(size_t l, size_t r);
    static std::shared_ptr<Node> NewNode(size_t l, size_t r, size_t enter);
    void DFS(std::vector<size_t>& res, std::shared_ptr<Node> current);

public:
    explicit SuffixTrie(const std::string& inputText);
    std::vector<size_t> Search(const std::string& pattern);
};

