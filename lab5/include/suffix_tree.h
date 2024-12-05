#pragma once 

#include <memory>
#include <map>
#include <vector>
#include <string>

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

