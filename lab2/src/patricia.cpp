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
    if (node->children[1] != nullptr && node->children[1]->bitNumber > node->bitNumber) {
        DestroyTrie(node->children[1]);
    }
    delete node;
}

TPatriciaTrie::~TPatriciaTrie() {
    DestroyTrie(root);
}

// if bitNumber == -1, it just finds node by key as usual; otherwise, it finds a place for new node with bitNumber
TPair<TPatriciaTrie::TNode*, int> TPatriciaTrie::FindPreviousNode(const std::string& key, int bitNumber) {
    if (root == nullptr) {
        return {root, -1};
    }
    TNode* previous = root;
    int currentDifferentBit = 0;
    TNode* current = previous->children[currentDifferentBit];
    while (current->bitNumber > previous->bitNumber && (current->bitNumber < bitNumber || bitNumber == -1)) {
        currentDifferentBit = GetBitByIndex(key, current->bitNumber);
        previous = current;
        current = previous->children[currentDifferentBit];
    }
    return {previous, currentDifferentBit};
}

// if bitNumber == -1, it just finds node by key as usual; otherwise, it finds a place for new node with bitNumber
TPatriciaTrie::TNode*& TPatriciaTrie::FindNode(const std::string& key, int bitNumber) {
    TPair<TNode*, int> previous = FindPreviousNode(key, bitNumber);
    if (previous.key == nullptr) {
        return root;
    }
    return previous.key->children[previous.value];
}

void TPatriciaTrie::Insert(const TData& data) {
    if (root == nullptr) {
        TNode* inserting = new TNode{data};
        root = inserting;
        root->children[0] = root;
        root->bitNumber = -1;
        ++size;
        return;
    }
    TNode* found = FindNode(data.key, -1);
    int bitIndex = BitDifference(data.key, found->data.key);
    if (bitIndex == -1) {
        throw std::logic_error("Exist");
    }
    TNode* inserting = new TNode{data};
    inserting->bitNumber = bitIndex;
    int differentBit = GetBitByIndex(data.key, bitIndex);
    TNode*& place = FindNode(data.key, bitIndex);
    inserting->children[differentBit] = inserting;
    inserting->children[!differentBit] = place;
    place = inserting;
    ++size;
}

const TPatriciaTrie::TData& TPatriciaTrie::Find(const std::string& key) {
    TNode* found = FindNode(key, -1);
    if (found == nullptr || BitDifference(key, found->data.key) != -1) {
        throw std::logic_error("NoSuchWord");
    }
    return found->data;
}

void TPatriciaTrie::Erase(const std::string& key) {
    TNode*& deleting = FindNode(key, -1);
    if (deleting == nullptr) {
        throw std::logic_error("NoSuchWord");
    }
    if (BitDifference(key, deleting->data.key) != -1) {
        throw std::logic_error("NoSuchWord");
    }
    --size;
    if (deleting->children[0] == deleting || deleting->children[1] == deleting) {
        // deleting has selfpointer
        if (deleting == root) {
            delete deleting;
            root = nullptr;
            return;
        }
        TNode*& deletingParentPointer = FindNode(key, deleting->bitNumber);
        if (deleting == deleting->children[0]) {
            deletingParentPointer = deleting->children[1];   // non-self pointer
            delete deleting;
            return;
        }
        deletingParentPointer = deleting->children[0];      // non-self pointer
        delete deleting;
        return;
    }
    // deleting does not have selfpointer
    // the node q has a backward pointer to node deleted
    TPair<TNode*, int> q = FindPreviousNode(key, -1);
    TNode*& backwardPointerToQ = FindNode(q.key->data.key, -1);
    TNode*& parentPointerToQ = FindNode(q.key->data.key, q.key->bitNumber);
    deleting->data = std::move(q.key->data);
    backwardPointerToQ = deleting;
    parentPointerToQ = q.key->children[!q.value];
    delete q.key;
}

int TPatriciaTrie::Size() const {
    return size;
}

void TPatriciaTrie::TreeToArray(TNode** array, TNode* root, int& id) const {
    if (root == nullptr) {
        return;
    }
    array[id] = root;
    root->id = id;
    if (root->children[0] != nullptr && root->children[0]->bitNumber > root->bitNumber) {
        TreeToArray(array, root->children[0], ++id);
    }
    if (root->children[1] != nullptr && root->children[1]->bitNumber > root->bitNumber) {
        TreeToArray(array, root->children[1], ++id);
    }
}

void TPatriciaTrie::SaveToFile(std::ofstream& file) const {
    file.write(reinterpret_cast<const char*>(&size), sizeof(int));
    TNode* nodes[size];
    int id = 0;
    TreeToArray(nodes, root, id);
    for (int i = 0; i < size; ++i) {
        TSaveData data;
        data.id = i;
        std::memcpy(data.key, nodes[i]->data.key.c_str(), sizeof(char) * KEY_LENGTH);
        data.value = nodes[i]->data.value;
        data.bitNumber = nodes[i]->bitNumber;
        data.leftId = nodes[i]->children[0]->id;
        if (i == 0) {    // root
            data.rightId = -1;
        } else {
            data.rightId = nodes[i]->children[1]->id;
        }
        file.write(reinterpret_cast<const char*>(&data), sizeof(TSaveData));
    }
}

void TPatriciaTrie::ArrayToTree(TSaveData* array) {
    TNode* nodes[size];
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
}

void TPatriciaTrie::LoadFromFile(std::ifstream& file) {
    DestroyTrie(root);
    root = nullptr;
    file.read(reinterpret_cast<char*>(&size), sizeof(int));
    if (file.fail()) {
        throw std::runtime_error("ERROR: Bad file");
    }
    if (size == 0) {
        return;
    }
    TSaveData datas[size];
    for (int i = 0; i < size; ++i) {
        file.read(reinterpret_cast<char*>(&datas[i]), sizeof(TSaveData));
        if (file.fail()) {
            throw std::runtime_error("ERROR: Bad file");
        }
    }
    ArrayToTree(datas);
}
