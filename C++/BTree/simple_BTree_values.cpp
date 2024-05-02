#include <iostream>
#include <vector>
#include <algorithm>

template <typename T, int B, typename Comparator = std::less<T>>
class BTreeNode {
public:
    std::vector<T> keys;
    std::vector<BTreeNode*> children;
    bool leaf;
    int childKeySum;

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
    void remove(const T& key);
    BTreeNode<T, B, Comparator>* search(const T& key);
    void print();
    BTreeNode<T, B, Comparator>* findParent(BTreeNode<T, B, Comparator>* node, BTreeNode<T, B, Comparator>* child);

private:
    void remove(BTreeNode<T, B, Comparator>* node, const T& key);
    void merge(BTreeNode<T, B, Comparator>* node, int index);
    void fill(BTreeNode<T, B, Comparator>* node, int index);
    void borrowFromPrev(BTreeNode<T, B, Comparator>* node, int index);
    void borrowFromNext(BTreeNode<T, B, Comparator>* node, int index);
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

        // Update childKeySum after the recursive call
        childKeySum = 0;
        for (const auto& child : children)
            childKeySum += child->keys.size();
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

    childKeySum = 0;
    for (const auto& child : children)
        childKeySum += child->keys.size();
}

template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::insert(const T& key) {
    if (root == nullptr) {
        root = new BTreeNode<T, B, Comparator>(true);
        root->keys.push_back(key);
        return;
    }

    BTreeNode<T, B, Comparator>* node = root;
    while (!node->leaf) {
        int i = std::upper_bound(node->keys.begin(), node->keys.end(), key, comp) - node->keys.begin();
        node = node->children[i];
    }

    node->keys.push_back(key);
    std::sort(node->keys.begin(), node->keys.end(), comp);

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

template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::remove(const T& key) {
    remove(root, key);
}
template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::remove(BTreeNode<T, B, Comparator>* node, const T& key) {
    if (node == nullptr)
        return;

    int i = std::lower_bound(node->keys.begin(), node->keys.end(), key, comp) - node->keys.begin();

    if (i < node->keys.size() && !(comp(key, node->keys[i]) || comp(node->keys[i], key))) {
        if (node->leaf) {
            node->keys.erase(node->keys.begin() + i);
        } else {
            BTreeNode<T, B, Comparator>* leftChild = node->children[i];
            BTreeNode<T, B, Comparator>* rightChild = node->children[i + 1];

            if (leftChild->keys.size() >= (B + 1) / 2) {
                T predKey = leftChild->keys.back();
                node->keys[i] = predKey;
                remove(leftChild, predKey);
            } else if (rightChild->keys.size() >= (B + 1) / 2) {
                T succKey = rightChild->keys.front();
                node->keys[i] = succKey;
                remove(rightChild, succKey);
            } else {
                merge(node, i);
                remove(leftChild, key);
            }

            // Update childKeySum after removal or merging
            node->childKeySum = 0;
            for (const auto& child : node->children)
                node->childKeySum += child->keys.size();
        }
    } else {
        if (node->leaf)
            return;

        bool flag = (i == node->keys.size());

        BTreeNode<T, B, Comparator>* child = node->children[i];

        if (child->keys.size() < (B + 1) / 2) {
            fill(node, i);
            // Update childKeySum after filling
            node->childKeySum = 0;
            for (const auto& child : node->children)
                node->childKeySum += child->keys.size();
        }

        if (flag && i > node->keys.size())
            remove(node->children[i - 1], key);
        else
            remove(node->children[i], key);
    }

    if (node->keys.empty() && node == root) {
        if (node->leaf) {
            delete root;
            root = nullptr;
        } else if (node->keys.empty()) {
            root = node->children.front();
            delete node;
        }
    }
}

template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::merge(BTreeNode<T, B, Comparator>* node, int index) {
    BTreeNode<T, B, Comparator>* leftChild = node->children[index];
    BTreeNode<T, B, Comparator>* rightChild = node->children[index + 1];

    leftChild->keys.push_back(node->keys[index]);
    node->keys.erase(node->keys.begin() + index);

    for (const auto& key : rightChild->keys)
        leftChild->keys.push_back(key);

    if (!leftChild->leaf) {
        for (auto& child : rightChild->children)
            leftChild->children.push_back(child);
    }

    node->children.erase(node->children.begin() + index + 1);
    delete rightChild;

    // Update childKeySum of the parent node after merging
    node->childKeySum = 0;
    for (const auto& child : node->children)
        node->childKeySum += child->keys.size();
}

template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::fill(BTreeNode<T, B, Comparator>* node, int index) {
    if (index != 0 && node->children[index - 1]->keys.size() >= (B + 1) / 2)
        borrowFromPrev(node, index);
    else if (index != node->keys.size() && node->children[index + 1]->keys.size() >= (B + 1) / 2)
        borrowFromNext(node, index);
    else {
        if ((node->childKeySum - 1 < B) &&  (node->childKeySum - 1 >= (B + 1) / 2)) return;
        if (index != node->keys.size())
            merge(node, index);
        else
            merge(node, index - 1);
    }
}

template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::borrowFromPrev(BTreeNode<T, B, Comparator>* node, int index) {
    BTreeNode<T, B, Comparator>* child = node->children[index];
    BTreeNode<T, B, Comparator>* sibling = node->children[index - 1];

    child->keys.insert(child->keys.begin(), node->keys[index - 1]);

    if (!child->leaf)
        child->children.insert(child->children.begin(), sibling->children.back());

    node->keys[index - 1] = sibling->keys.back();
    sibling->keys.pop_back();

    if (!sibling->leaf)
        sibling->children.pop_back();

    // Update childKeySum of the parent node after borrowing
    node->childKeySum = 0;
    for (const auto& child : node->children)
        node->childKeySum += child->keys.size();
}
template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::borrowFromNext(BTreeNode<T, B, Comparator>* node, int index) {
    BTreeNode<T, B, Comparator>* child = node->children[index];
    BTreeNode<T, B, Comparator>* sibling = node->children[index + 1];

    child->keys.push_back(node->keys[index]);

    if (!child->leaf)
        child->children.push_back(sibling->children.front());

    node->keys[index] = sibling->keys.front();
    sibling->keys.erase(sibling->keys.begin());

    if (!sibling->leaf)
        sibling->children.erase(sibling->children.begin());

    // Update childKeySum of the parent node after borrowing
    node->childKeySum = 0;
    for (const auto& child : node->children)
        node->childKeySum += child->keys.size();
}


int main() {
    BTree<int, 3> tree;

    // Insert keys
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(60);
    tree.insert(70);
    tree.insert(80);
    tree.insert(90);

    // Print the tree
    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;

    // Remove keys
    tree.remove(30);
    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;
    tree.remove(50);
    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;
    tree.remove(70);

    // Print the tree after removal
    std::cout << "B-Tree after removal:" << std::endl;
    tree.print();
    std::cout << std::endl;

    // Search for a key
    int key = 40;
    BTreeNode<int, 3>* result = tree.search(key);
    if (result != nullptr)
        std::cout << "Key " << key << " found in the tree" << std::endl;
    else
        std::cout << "Key " << key << " not found in the tree" << std::endl;

    return 0;
}