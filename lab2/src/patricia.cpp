#include "patricia.h"

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
    if (fread(reinterpret_cast<char*>(datas), sizeof(TSaveData), size, file) < size) {
        return false;
    }
    ArrayToTree(datas);
    delete[] datas;
    return true;
}
