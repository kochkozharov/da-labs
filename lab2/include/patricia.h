#pragma once

#include "binary_string.h"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <cstdint>

template <class T, class U>
struct TPair {
    T key;
    U value;

    TPair() = default;
    TPair(const T& key, const U& value) : key(key), value(value) {}
    TPair(T&& key, U&& value) : key(key), value(value) {}
    TPair(const TPair& other) : key(other.key), value(other.value) {}
    TPair(TPair&& other) noexcept : key(std::move(other.key)), value(std::move(other.value)) {}
    TPair& operator=(const TPair& other) {
        key = other.key;
        value = other.value;
        return *this;
    }
    TPair& operator=(TPair&& other) noexcept {
        key = std::move(other.key);
        value = std::move(other.value);
        return *this;
    }
};

class TPatriciaTrie {
private:
    using TData = TPair<std::string, uint64_t>;

    static const int KEY_LENGTH = 256;

    struct TNode {
        TData data;
        TNode* children[2];
        int bitNumber;
        int id;

        TNode() = default;
        TNode(const TData& data);
    };

    struct TSaveData {
        int id;
        char key[KEY_LENGTH];
        uint64_t value;
        int bitNumber;
        int leftId;
        int rightId;

        TSaveData() = default;
    };

    TNode* root;
    int size;

    TNode*& FindNode(const std::string& key, int bitNumber);
    TPair<TPatriciaTrie::TNode*, int> FindPreviousNode(const std::string& key, int bitNumber);
    void DestroyTrie(TNode* node);
    void TreeToArray(TNode** array, TNode* root, int& id) const;
    void ArrayToTree(TSaveData* array);
public:
    TPatriciaTrie();
    ~TPatriciaTrie();
    void Insert(const TData& data);
    const TData& Find(const std::string& key);
    void Erase(const std::string& key);
    void SaveToFile(std::ofstream& file) const;
    void LoadFromFile(std::ifstream& file);
    int Size() const;
};
