#pragma once
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>

#include "binary_string.h"

struct CFile {
    enum class FileType { Save, Load };
    FILE* file;
    CaseInsensitiveString name;
    CFile(CaseInsensitiveString s, FileType mode) {
        name = s;
        if (mode == FileType::Save) {
            file = fopen(s.CStr(), "wb");
        }
        else {
            file = fopen(s.CStr(), "rb");
        }
        if (file == nullptr) {
            throw std::logic_error("Bad File");
        }
    }
    FILE* GetFile() { return file; }
    CFile() = delete;
    CFile(const CFile& other) = delete;
    CFile(CFile&& other) = delete;
    CFile& operator=(const CFile& other) = delete;
    CFile& operator=(CFile&& other) = delete;
    ~CFile() {
        fclose(file);
        //remove(name.CStr());
    }
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
    using TData = TPair<CaseInsensitiveString, uint64_t>;

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
        TData keyValue;
        int bitNumber;
        int leftId;
        int rightId;

        TSaveData() = default;
    };

    TNode* root;
    int size;

    TNode*& FindNode(const CaseInsensitiveString& key, int bitNumber);
    TPair<TPatriciaTrie::TNode*, int> FindPreviousNode(
        const CaseInsensitiveString& key, int bitNumber);
    void DestroyTrie(TNode* node);
    void TreeToArray(TNode** array, TNode* root, int& id) const;
    void ArrayToTree(TSaveData* array);

   public:
    TPatriciaTrie();
    ~TPatriciaTrie();
    void Insert(const TData& data);
    const TData& Find(const CaseInsensitiveString& key);
    void Erase(const CaseInsensitiveString& key);
    void SaveToFile(FILE *file) const;
    void LoadFromFile(FILE *file);
    int Size() const;
};
