#include <cctype>
#include <cinttypes>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <utility>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

struct TCaseInsensitiveString {
   private:
    static const int KEY_LENGTH = 256;
    char str[KEY_LENGTH + 1];
    int sz = 0;

   public:
    TCaseInsensitiveString() = default;
    TCaseInsensitiveString(const char* value) {
        for (int i = 0; i < KEY_LENGTH && value[i] != '\0'; ++i) {
            str[i] = std::tolower(value[i]);
            sz++;
        }
        str[sz] = '\0';
    }
    const char& operator[](int idx) const { return str[idx]; }
    int size() const { return sz; }
    bool Scan(FILE* stream = stdin) {
        int res = fscanf(stream, "%256s", str);
        if (res == EOF) {
            return false;
        }
        sz = strlen(str);
        for (int i = 0; i < sz; ++i) {
            str[i] = std::tolower(str[i]);
        }
        return true;
    }
    const char* CStr() const { return str; }
    friend bool operator==(const TCaseInsensitiveString& a,
                           const TCaseInsensitiveString& b) {
        return strcmp(a.str, b.str) == 0;
    }
};

int GetBitSize(const TCaseInsensitiveString& str);

int GetBitDifference(const TCaseInsensitiveString& a,
                  const TCaseInsensitiveString& b);

bool GetBitByIndex(const TCaseInsensitiveString& str, int index);

int GetBitSize(const TCaseInsensitiveString& str) {
    return str.size() * CHAR_BIT;
}

bool GetBitByIndex(const TCaseInsensitiveString& str, int index) {
    const char* ptr = str.CStr();
    int byteIndex = index / CHAR_BIT;
    int bitIndex = index % CHAR_BIT;
    return ptr[byteIndex] & (1 << bitIndex);
}

int GetBitDifference(const TCaseInsensitiveString& a,
                  const TCaseInsensitiveString& b) {
    int n = 0;
    for (int i = 0; i < MAX(GetBitSize(a), GetBitSize(b)); ++i) {
        if (GetBitByIndex(a, i) != GetBitByIndex(b, i)) {
            break;
        }
        n++;
    }
    if (a.size() == b.size() && n == GetBitSize(a)) {
        return -1;
    }
    return n;
}

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
    bool check() { return file != nullptr; }
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
    const TData* Find(const TCaseInsensitiveString& key);
    bool Erase(const TCaseInsensitiveString& key);
    bool SaveToFile(FILE* file) const;
    bool LoadFromFile(FILE* file);
    int Size() const;
};

TPatriciaTrie::TNode::TNode(const TData& data) : data{data} {
    children[0] = nullptr;
    children[1] = nullptr;
}

TPatriciaTrie::TPatriciaTrie() : root{nullptr}, size{0} {}

void TPatriciaTrie::DestroyTrie(TNode* node) {
    if (node == nullptr) {
        return;
    }
    if (node->children[0]->bitNumber > node->bitNumber) {
        DestroyTrie(node->children[0]);
    }
    if (node->children[1] != nullptr &&
        node->children[1]->bitNumber > node->bitNumber) {
        DestroyTrie(node->children[1]);
    }
    delete node;
}

TPatriciaTrie::~TPatriciaTrie() { DestroyTrie(root); }

TPair<TPatriciaTrie::TNode*, int> TPatriciaTrie::FindPreviousNode(
    const TCaseInsensitiveString& key, int bitNumber) {
    if (root == nullptr) {
        return {root, -1};
    }
    TNode* previous = root;
    int currentDifferentBit = 0;
    TNode* current = previous->children[currentDifferentBit];
    while (current->bitNumber > previous->bitNumber &&
           (current->bitNumber < bitNumber || bitNumber == -1)) {
        currentDifferentBit = GetBitByIndex(key, current->bitNumber);
        previous = current;
        current = previous->children[currentDifferentBit];
    }
    return {previous, currentDifferentBit};
}

TPatriciaTrie::TNode*& TPatriciaTrie::FindNode(const TCaseInsensitiveString& key,
                                               int bitNumber) {
    TPair<TNode*, int> previous = FindPreviousNode(key, bitNumber);
    if (previous.key == nullptr) {
        return root;
    }
    return previous.key->children[previous.value];
}

bool TPatriciaTrie::Insert(const TData& data) {
    if (root == nullptr) {
        TNode* inserting = new TNode{data};
        root = inserting;
        root->children[0] = root;
        root->bitNumber = -1;
        ++size;
        return true;
    }
    TNode* found = FindNode(data.key, -1);
    int bitIndex = GetBitDifference(data.key, found->data.key);
    if (bitIndex == -1) {
        return false;
    }
    TNode* inserting = new TNode{data};
    inserting->bitNumber = bitIndex;
    int differentBit = GetBitByIndex(data.key, bitIndex);
    TNode*& place = FindNode(data.key, bitIndex);
    inserting->children[differentBit] = inserting;
    inserting->children[!differentBit] = place;
    place = inserting;
    ++size;
    return true;
}

const TPatriciaTrie::TData* TPatriciaTrie::Find(
    const TCaseInsensitiveString& key) {
    TNode* found = FindNode(key, -1);
    if (found == nullptr || GetBitDifference(key, found->data.key) != -1) {
        return nullptr;
    }
    return &found->data;
}

bool TPatriciaTrie::Erase(const TCaseInsensitiveString& key) {
    TNode*& deleting = FindNode(key, -1);
    if (deleting == nullptr) {
        return false;
    }
    if (GetBitDifference(key, deleting->data.key) != -1) {
        return false;
    }
    --size;
    if (deleting->children[0] == deleting ||
        deleting->children[1] == deleting) {
        if (deleting == root) {
            delete deleting;
            root = nullptr;
            return true;
        }
        TNode*& deletingParentPointer = FindNode(key, deleting->bitNumber);
        if (deleting == deleting->children[0]) {
            deletingParentPointer = deleting->children[1];  // non-self pointer
            delete deleting;
            return true;
        }
        deletingParentPointer = deleting->children[0];  // non-self pointer
        delete deleting;
        return true;
    }
    TPair<TNode*, int> q = FindPreviousNode(key, -1);
    TNode*& backwardPointerToQ = FindNode(q.key->data.key, -1);
    TNode*& parentPointerToQ = FindNode(q.key->data.key, q.key->bitNumber);
    deleting->data = std::move(q.key->data);
    backwardPointerToQ = deleting;
    parentPointerToQ = q.key->children[!q.value];
    delete q.key;
    return true;
}

int TPatriciaTrie::Size() const { return size; }

void TPatriciaTrie::TreeToArray(TNode** array, TNode* root, int& id) const {
    if (root == nullptr) {
        return;
    }
    array[id] = root;
    root->id = id;
    if (root->children[0] != nullptr &&
        root->children[0]->bitNumber > root->bitNumber) {
        TreeToArray(array, root->children[0], ++id);
    }
    if (root->children[1] != nullptr &&
        root->children[1]->bitNumber > root->bitNumber) {
        TreeToArray(array, root->children[1], ++id);
    }
}

bool TPatriciaTrie::SaveToFile(FILE* file) const {
    if (fwrite(reinterpret_cast<const char*>(&size), sizeof(int), 1, file) <
        1) {
        return false;
    };
    TNode** nodes = new TNode*[size];
    int id = 0;
    TreeToArray(nodes, root, id);
    for (int i = 0; i < size; ++i) {
        TSaveData data;
        memset(&data, 0, sizeof(data));
        data.id = i;
        memcpy(data.key, nodes[i]->data.key.CStr(), nodes[i]->data.key.size());
        data.value = nodes[i]->data.value;
        data.bitNumber = nodes[i]->bitNumber;
        data.leftId = nodes[i]->children[0]->id;
        if (i == 0) {  // root
            data.rightId = -1;
        } else {
            data.rightId = nodes[i]->children[1]->id;
        }
        if (fwrite(reinterpret_cast<const char*>(&data), sizeof(TSaveData), 1,
                   file) < 1) {
            return false;
        };
    }
    delete[] nodes;
    return true;
}

void TPatriciaTrie::ArrayToTree(TSaveData* array) {
    TNode** nodes = new TNode*[size];

    for (int i = 0; i < size; ++i) {
        nodes[i] = new TNode;
        nodes[i]->data = {array[i].key, array[i].value};
        nodes[i]->bitNumber = array[i].bitNumber;
    }

    for (int i = 0; i < size; ++i) {
        nodes[i]->children[0] = nodes[array[i].leftId];
        if (array[i].rightId == -1) {
            nodes[i]->children[1] = nullptr;
        } else {
            nodes[i]->children[1] = nodes[array[i].rightId];
        }
    }
    root = nodes[0];

    delete[] nodes;
}

bool TPatriciaTrie::LoadFromFile(FILE* file) {
    DestroyTrie(root);
    root = nullptr;
    if (fread(reinterpret_cast<char*>(&size), sizeof(int), 1, file) < 1) {
        return false;
    }
    if (size == 0) {
        return true;
    }
    TSaveData* datas = new TSaveData[size];
    if (fread(reinterpret_cast<char*>(datas), sizeof(TSaveData), size, file) <
        size) {
        return false;
    }
    ArrayToTree(datas);
    delete[] datas;
    return true;
}

int main() {
    TCaseInsensitiveString input;
    TPatriciaTrie t;
    while (input.Scan()) {
        if (input == "+") {
            TCaseInsensitiveString key;
            uint64_t value;
            key.Scan();
            scanf("%" SCNu64, &value);
            if (!t.Insert({key, value})) {
                printf("Exist\n");
                continue;
            }
        } else if (input == "-") {
            TCaseInsensitiveString key;
            key.Scan();
            if (!t.Erase(key)) {
                printf("NoSuchWord\n");
                continue;
            }
        } else if (input == "!") {
            TCaseInsensitiveString cmd;
            cmd.Scan();
            if (cmd == "Save") {
                TCaseInsensitiveString path;
                path.Scan();
                TFile file(path, TFile::FileType::Save);
                if (!file.check()) {
                    fprintf(stderr, "ERROR: Bad File\n");
                    continue;
                }
                if (!t.SaveToFile(file.GetFile())) {
                    fprintf(stderr, "ERROR: Bad File\n");
                    continue;
                }
            } else if (cmd == "Load") {
                TCaseInsensitiveString path;
                path.Scan();
                TFile file(path, TFile::FileType::Load);
                if (!file.check()) {
                    fprintf(stderr, "ERROR: Bad File\n");
                    continue;
                }
                if (!t.LoadFromFile(file.GetFile())) {
                    fprintf(stderr, "ERROR: Bad File\n");
                    continue;
                }
            }
        } else {
            const TPair<TCaseInsensitiveString, uint64_t>* p;
            p = t.Find(input.CStr());
            if (!p) {
                printf("NoSuchWord\n");
                continue;
            }
            printf("OK: %" PRIu64 "\n", p->value);
            continue;
        }
        printf("OK\n");
    }
    return 0;
}