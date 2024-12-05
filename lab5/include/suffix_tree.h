#pragma once 

#include <memory>
#include <map>
#include <vector>
#include <string>

class Trie {
private:
    struct Vertex {
        int l{-1};
        int r{-1};
        int enter{-1};
        std::map<char, std::shared_ptr<Vertex>> child;
    };

    std::shared_ptr<Vertex> root;
    std::string text;

    void Insert(int l, int r);
    static std::shared_ptr<Vertex> NewVertex(int l, int r);
    static std::shared_ptr<Vertex> NewVertex(int l, int r, int enter);
    void DFS(std::vector<int>& res, std::shared_ptr<Vertex> current);

public:
    explicit Trie(const std::string& inputText);
    std::vector<int> Search(const std::string& pattern);
};

