#include <iostream>
#include <vector>
#include <algorithm>


template <typename Iter, typename T, typename Comparator>
int binarySearch(Iter begin, Iter end, const T& key, Comparator comp) {
    auto it = std::lower_bound(begin, end, key, comp);
    return std::distance(begin, it);
}

template <typename T, int B, typename Comparator = std::less<T>>
class BTreeNode {
public:
    std::vector<T> keys;
    std::vector<BTreeNode*> children;
    bool leaf;

    struct BTreeNodeComparator {
        Comparator comp;

        bool operator()(const BTreeNode<T, B, Comparator>* node, const T& key) const {
            return comp(node->keys.front(), key);
        }

        bool operator()(const T& key, const BTreeNode<T, B, Comparator>* node) const {
            return comp(key, node->keys.front());
        }
    };

    BTreeNode(bool leaf) : leaf(leaf) {}

    void insertNonFull(const T& key, Comparator comp);
    void splitChild(int index, BTreeNode* child, Comparator comp);
    void print(int level);



};

template <typename T, int B, typename Comparator = std::less<T>>
class BTree {
private:
    BTreeNode<T, B, Comparator>* root;
    Comparator comp;

public:
    BTree() : root(nullptr) {}

    void insert(const T& key);
    BTreeNode<T, B, Comparator>* search(const T& key);
    void print();
    BTreeNode<T, B, Comparator>* findParent(BTreeNode<T, B, Comparator>* node, BTreeNode<T, B, Comparator>* child);
};



template <typename T, int B, typename Comparator>
void BTreeNode<T, B, Comparator>::insertNonFull(const T& key, Comparator comp) {
    int i = keys.size() - 1;

    if (leaf) {
        keys.resize(keys.size() + 1);
        while (i >= 0 && comp(key, keys[i])) {
            keys[i + 1] = keys[i];
            --i;
        }
        keys[i + 1] = key;
    } else {
        while (i >= 0 && comp(key, keys[i]))
            --i;

        ++i;

        if (children[i]->keys.size() == B - 1) {
            splitChild(i, children[i], comp);
            if (comp(keys[i], key))
                ++i;
        }

        children[i]->insertNonFull(key, comp);
    }
}

template <typename T, int B, typename Comparator>
void BTreeNode<T, B, Comparator>::splitChild(int index, BTreeNode* child, Comparator comp) {
    BTreeNode* newChild = new BTreeNode(child->leaf);
    newChild->keys.resize((B - 1) / 2);

    for (int i = 0; i < (B - 1) / 2; ++i)
        newChild->keys[i] = child->keys[i + (B - 1) / 2];

    if (!child->leaf) {
        newChild->children.resize((B + 1) / 2);
        for (int i = 0; i < (B + 1) / 2; ++i)
            newChild->children[i] = child->children[i + (B + 1) / 2];
        child->children.resize(child->children.size() - (B - 1) / 2);
    }

    children.insert(children.begin() + index + 1, newChild);
    keys.insert(keys.begin() + index, child->keys[(B - 1) / 2]);
}

template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::insert(const T& key) {
    if (root == nullptr) {
        root = new BTreeNode<T, B, Comparator>(false);
        BTreeNode<T, B, Comparator>* leaf = new BTreeNode<T, B, Comparator>(true);
        root->keys.push_back(key);
        leaf->keys.push_back(key);
        root->children.push_back(leaf);
        return;
    }

    BTreeNode<T, B, Comparator>* node = root;
    int i = 0;
    do {
        i = std::upper_bound(node->keys.begin(), node->keys.end(), key, comp) - node->keys.begin();
    } while(!node->children[i]->leaf && (node = node->children[i]));

    BTreeNode<T, B, Comparator>* leaf = new BTreeNode<T, B, Comparator>(true);
    leaf->keys.push_back(key);
    int indexBeforeInsert = binarySearch(
            node->children.begin(),node->children.end(), key,
            typename BTreeNode<T, B, Comparator>::BTreeNodeComparator{comp}
            );
    node->children.emplace(node->children.begin() + indexBeforeInsert, leaf);
    //std::sort(node->keys.begin(), node->keys.end(), comp);

    if(node->children.size() - 1 != node->keys.size()){
        int idx = binarySearch(node->keys.begin(), node->keys.begin(), key, comp);
        node->keys.emplace(node->keys.begin() + idx, key);
    }

    BTreeNode<T, B, Comparator>* inserted = node;


    while (node != root && node->keys.size() > B - 1) {
        BTreeNode<T, B, Comparator>* parent = findParent(root, node);
        int splitIndex = (B - 1) / 2;
        T splitKey = node->keys[splitIndex];

        BTreeNode<T, B, Comparator>* newNode = new BTreeNode<T, B, Comparator>(node->leaf);
        newNode->keys.assign(node->keys.begin() + splitIndex + 1, node->keys.end());
        node->keys.erase(node->keys.begin() + splitIndex, node->keys.end());

        if (!node->leaf) {
            newNode->children.assign(node->children.begin() + splitIndex + 1, node->children.end());
            node->children.erase(node->children.begin() + splitIndex + 1, node->children.end());
        }

        int insertIndex = std::upper_bound(parent->keys.begin(), parent->keys.end(), splitKey, comp) - parent->keys.begin();
        parent->keys.insert(parent->keys.begin() + insertIndex, splitKey);
        parent->children.insert(parent->children.begin() + insertIndex + 1, newNode);

        node = parent;
    }
    //if(std::count(inserted->keys.begin(), inserted->keys.end(), key) == 2){
    //    inserted->keys.erase(std::find(inserted->keys.begin(), inserted->keys.end(), key));
    //}

    if (node->keys.size() > B - 1) {
        BTreeNode<T, B, Comparator>* newRoot = new BTreeNode<T, B, Comparator>(false);
        int splitIndex = (B - 1) / 2;
        T splitKey = node->keys[splitIndex];

        BTreeNode<T, B, Comparator>* newNode = new BTreeNode<T, B, Comparator>(node->leaf);
        newNode->keys.assign(node->keys.begin() + splitIndex + 1, node->keys.end());
        node->keys.erase(node->keys.begin() + splitIndex, node->keys.end());

        if (!node->leaf) {
            newNode->children.assign(node->children.begin() + splitIndex + 1, node->children.end());
            node->children.erase(node->children.begin() + splitIndex + 1, node->children.end());
        }

        newRoot->keys.push_back(splitKey);
        newRoot->children.push_back(node);
        newRoot->children.push_back(newNode);

        root = newRoot;
    }
}

template <typename T, int B, typename Comparator>
BTreeNode<T, B, Comparator>* BTree<T, B, Comparator>::findParent(BTreeNode<T, B, Comparator>* node, BTreeNode<T, B, Comparator>* child) {
    if (node->leaf)
        return nullptr;

    if (std::find(node->children.begin(), node->children.end(), child) != node->children.end())
        return node;

    for (auto& childNode : node->children) {
        BTreeNode<T, B, Comparator>* parent = findParent(childNode, child);
        if (parent != nullptr)
            return parent;
    }

    return nullptr;
}

template <typename T, int B, typename Comparator>
BTreeNode<T, B, Comparator>* BTree<T, B, Comparator>::search(const T& key) {
    BTreeNode<T, B, Comparator>* node = root;

    while (node != nullptr) {
        int i = std::lower_bound(node->keys.begin(), node->keys.end(), key, comp) - node->keys.begin();

        if (i < node->keys.size() && !(comp(key, node->keys[i]) || comp(node->keys[i], key)))
            return node;

        if (node->leaf)
            break;

        node = node->children[i];
    }

    return nullptr;
}

template <typename T, int B, typename Comparator>
void BTreeNode<T, B, Comparator>::print(int level) {
    std::cout << std::string(level * 2, ' ');
    std::cout << "Keys: ";
    for (const auto& key : keys)
        std::cout << key << " " << "isLeaf " << (this->leaf ? "True " : "False ");
    std::cout << std::endl;

    if (!leaf) {
        for (const auto& child : children)
            child->print(level + 1);
    }
}

template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::print() {
    if (root != nullptr)
        root->print(0);
    else
        std::cout << "Empty tree" << std::endl;
}

int main() {
    BTree<int, 3> tree;

    // Insert keys
    tree.insert(3);
    tree.insert(2);
    tree.insert(1);
    tree.insert(10);
    tree.insert(14);
    tree.insert(25);
    tree.insert(22);

    // Print the tree
    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;
    // Search for a key
    int key = 1;
    BTreeNode<int, 3>* result = tree.search(key);
    if (result != nullptr)
        std::cout << "Key " << key << " found in the tree" << std::endl;
    else
        std::cout << "Key " << key << " not found in the tree" << std::endl;

    return 0;
}