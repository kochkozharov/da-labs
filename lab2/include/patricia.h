#pragma once
#include <cstdint>
#include <cstring>
#include <utility>

#include "binary_string.h"

struct TFile {
    enum class FileType { Save, Load };
    FILE* file;
    TCaseInsensitiveString name;
    TFile(TCaseInsensitiveString s, FileType mode) {
        name = s;
        if (mode == FileType::Save) {
            file = fopen(s.CStr(), "wb");
        } else {
            file = fopen(s.CStr(), "rb");
        }

    }
    bool check() {
        return file != nullptr;
    }
    FILE* GetFile() { return file; }
    TFile() = delete;
    TFile(const TFile& other) = delete;
    TFile(TFile&& other) = delete;
    TFile& operator=(const TFile& other) = delete;
    TFile& operator=(TFile&& other) = delete;
    ~TFile() { fclose(file); }
};

template <class T, class U>
struct TPair {
    T key;
    U value;

    TPair() = default;
    TPair(const T& key, const U& value) : key(key), value(value) {}
    TPair(T&& key, U&& value) : key(key), value(value) {}
    TPair(const TPair& other) : key(other.key), value(other.value) {}
    TPair(TPair&& other) noexcept
        : key(std::move(other.key)), value(std::move(other.value)) {}
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
    using TData = TPair<TCaseInsensitiveString, uint64_t>;

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
        char key[257];
        uint64_t value;
        int bitNumber;
        int leftId;
        int rightId;
    };

    TNode* root;
    int size;

    TNode*& FindNode(const TCaseInsensitiveString& key, int bitNumber);
    TPair<TPatriciaTrie::TNode*, int> FindPreviousNode(
        const TCaseInsensitiveString& key, int bitNumber);
    void DestroyTrie(TNode* node);
    void TreeToArray(TNode** array, TNode* root, int& id) const;
    void ArrayToTree(TSaveData* array);

   public:
    TPatriciaTrie();
    ~TPatriciaTrie();
    bool Insert(const TData& data);
    const TData *Find(const TCaseInsensitiveString& key);
    bool Erase(const TCaseInsensitiveString& key);
    bool SaveToFile(FILE* file) const;
    bool LoadFromFile(FILE* file);
    int Size() const;
};
