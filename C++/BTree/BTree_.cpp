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
    BTreeNode* getLeafParent(const T& key, int& i);
    void removeLeaf(int i);
};

template <typename T, int B, typename Comparator>
BTreeNode<T, B, Comparator>* BTreeNode<T, B, Comparator>::getLeafParent(const T& key, int& i) {
    //assert this != nullptr
    i = 0;
    BTreeNode<T, B, Comparator>* node = this;
    Comparator comp;
    do {
        i = std::upper_bound(node->keys.begin(), node->keys.end(), key, comp) - node->keys.begin();
    } while(!node->children[i]->leaf && (node = node->children[i]));

    if(comp(node->children[i]->keys.front(), key) || comp(node->children[i]->keys.front(), key)) return nullptr;
    return node;
}

template <typename T, int B, typename Comparator>
void BTreeNode<T, B, Comparator>::removeLeaf(int child_idx){
    //assert this and child_idx >= 0 and <= children.size()

    int distance = -1;
    Comparator comp;
    if(child_idx == 0) distance = 0;
    else if(child_idx == this->children.size() - 1) distance = this->children.size() - 2;
    else if(!comp(this->keys[child_idx-1], this->children[child_idx]->keys.front()) &&
            !comp(this->keys[child_idx-1], this->children[child_idx]->keys.front()))
        distance = child_idx - 1;
    else
        distance = child_idx;
    this->children[child_idx]->keys.erase(this->children[child_idx]->keys.begin());
    this->children.erase(this->children.begin() + child_idx);

    if( this->keys.size() > B / 2) this->keys.erase(this->keys.begin() + distance);
}


template <typename T, int B, typename Comparator = std::less<T>>
class BTree {
private:
    BTreeNode<T, B, Comparator>* root;
    Comparator comp;

    void remove(BTreeNode<T, B, Comparator>* node, const T& key);
    void merge(BTreeNode<T, B, Comparator>* node, int index);
    void fill(BTreeNode<T, B, Comparator>* node, int index);
    void borrowFromPrev(BTreeNode<T, B, Comparator>* node, int index);
    void borrowFromNext(BTreeNode<T, B, Comparator>* node, int index);

public:
    BTree() : root(nullptr) {}

    void insert(const T& key);
    BTreeNode<T, B, Comparator>* search(const T& key);
    void print();
    BTreeNode<T, B, Comparator>* findParent(BTreeNode<T, B, Comparator>* node, BTreeNode<T, B, Comparator>* child);
    void remove(const T& key);
};

template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::remove(const T& key) {
    if (root == nullptr)
        return;

    remove(root, key);

    if (root->keys.empty()) {
        BTreeNode<T, B, Comparator>* oldRoot = root;
        if (root->leaf)
            root = nullptr;
        else
            root = root->children[0];
        delete oldRoot;
    }
}

template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::remove(BTreeNode<T, B, Comparator>* r, const T& key) {


    BTreeNode<T, B, Comparator>* node = root;
    int child_idx = -1;
    BTreeNode<T, B, Comparator>* leafParent = root->getLeafParent(key, child_idx);

    leafParent->removeLeaf(child_idx);

    if(leafParent->children.size() > B / 2) return;

    T& smallestKey = leafParent->keys.front();
    T& biggestKey = leafParent->keys.back();

    BTreeNode<T, B, Comparator>* parent = findParent(root, leafParent);

    BTreeNode<T, B, Comparator>* leftBrother = nullptr;
    BTreeNode<T, B, Comparator>* rightBrother = nullptr;

    int leftBrotherIdx = -1;
    int rightBrotherIdx = -1;
    if(parent->children[0] != leafParent) {
        int t = std::lower_bound(parent->keys.begin(), parent->keys.end(), smallestKey, comp) - parent->keys.begin();
        leftBrotherIdx = t - 1;
        leftBrother = parent->children[leftBrotherIdx];
    }
    if(parent->children[parent->children.size() - 1] != leafParent) {
        int t = std::upper_bound(parent->keys.begin(), parent->keys.end(), biggestKey, comp) - parent->keys.begin();
        rightBrotherIdx = t + 1;
        rightBrother = parent->children[rightBrotherIdx];
    }
    if(leftBrother && leftBrother->children.size() > B / 2 + 1) {
        int i = 0;
        do {
            i = std::upper_bound(leftBrother->keys.begin(), leftBrother->keys.end(), key, comp) - leftBrother->keys.begin();
        } while(!leftBrother->children[i]->leaf && (leftBrother = leftBrother->children[i]));
        if(leftBrother->children.size() > B / 2 + 1) {
            T key =  leftBrother->children[i]->keys.front();
            leftBrother->removeLeaf(i);


            int parent_idx = std::upper_bound(
                    parent->keys.begin(), parent->keys.end(),
                    key, comp) - parent->keys.begin();

            parent->keys[parent_idx] = key;

            Comparator comp;
            if(comp(leafParent->children.front()->keys.front(), key)){
                leafParent->keys[0] = key;
            } else {
                leafParent->keys[0] = leafParent->children.front()->keys.front();
            }

            leafParent->insertNonFull(key, comp);

            return;
        }
    } else if (rightBrother && rightBrother->children.size() > B / 2 + 1) {
        int i = 0;
        do {
            i = std::lower_bound(rightBrother->keys.begin(), rightBrother->keys.end(), key, comp) - rightBrother->keys.begin();
        } while(!rightBrother->children[i]->leaf && (rightBrother = rightBrother->children[i]));
        if(rightBrother->children.size() > B / 2 + 1){
            T key = rightBrother->children[i]->keys.front();
            T parentKey = rightBrother->children[i+1]->keys.front();

            rightBrother->removeLeaf(i);
            leafParent->insertNonFull(key, comp);

            int parent_idx = std::lower_bound(
                    parent->keys.begin(), parent->keys.end(),
                    key, comp) - parent->keys.begin();

            parent->keys[parent_idx] = parentKey;

            leafParent->keys[0] = key;

            return ;
        }
    } else {
        // Merge operation
        if (leftBrother) {
            // Merge leafParent with leftBrother
            leftBrother->keys.insert(leftBrother->keys.end(), leafParent->keys.begin(), leafParent->keys.end());
            leftBrother->children.insert(leftBrother->children.end(), leafParent->children.begin(), leafParent->children.end());

            int parent_idx = std::lower_bound(parent->keys.begin(), parent->keys.end(), smallestKey, comp) - parent->keys.begin();
            //parent->keys.erase(parent->keys.begin() + parent_idx - 1);
            parent->children.erase(parent->children.begin() + parent_idx);

            delete leafParent;

            parent->keys.erase(parent->keys.begin() + leftBrotherIdx);


            //parent->fixAfterMerge();
        } else if (rightBrother) {
            // Merge leafParent with rightBrother
            leafParent->keys.insert(leafParent->keys.end(), rightBrother->keys.begin(), rightBrother->keys.end());
            leafParent->children.insert(leafParent->children.end(), rightBrother->children.begin(), rightBrother->children.end());

            int parent_idx = std::upper_bound(parent->keys.begin(), parent->keys.end(), biggestKey, comp) - parent->keys.begin();
            //parent->keys.erase(parent->keys.begin() + parent_idx);
            parent->children.erase(parent->children.begin() + parent_idx + 1);

            delete rightBrother;

            parent->keys.erase(parent->keys.begin() + rightBrotherIdx - 1);

            //parent->fixAfterMerge();
        }
    }

}

template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::fill(BTreeNode<T, B, Comparator>* node, int index) {
    if (index != 0 && node->children[index - 1]->keys.size() >= B / 2)
        borrowFromPrev(node, index);
    else if (index != node->keys.size() && node->children[index + 1]->keys.size() >= B / 2)
        borrowFromNext(node, index);
    else {
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
}

template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::borrowFromNext(BTreeNode<T, B, Comparator>* node, int index) {
    BTreeNode<T, B, Comparator>* child = node->children[index];
    BTreeNode<T, B, Comparator>* sibling = node->children[index + 1];

    child->keys.push_back(node->keys[index]);

    if (!child->leaf)
        child->children.push_back(sibling->children[0]);

    node->keys[index] = sibling->keys[0];
    sibling->keys.erase(sibling->keys.begin());

    if (!sibling->leaf)
        sibling->children.erase(sibling->children.begin());
}

template <typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::merge(BTreeNode<T, B, Comparator>* node, int index) {
    BTreeNode<T, B, Comparator>* child = node->children[index];
    BTreeNode<T, B, Comparator>* sibling = node->children[index + 1];

    child->keys.push_back(node->keys[index]);
    child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());

    if (!child->leaf)
        child->children.insert(child->children.end(), sibling->children.begin(), sibling->children.end());

    node->keys.erase(node->keys.begin() + index);
    node->children.erase(node->children.begin() + index + 1);

    delete sibling;
}



template <typename T, int B, typename Comparator>
void BTreeNode<T, B, Comparator>::insertNonFull(const T& key, Comparator comp) {
    int i = children.size() - 1;
    if (children.front()->leaf) {
        children.resize(children.size() + 1);
        while (i >= 0 && comp(key, children[i]->keys.front())) {
            children[i + 1] = children[i];
            --i;
        }
        children[i + 1] = new BTreeNode<T, B, Comparator>(true);
        children[i + 1]->keys.push_back(key);
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
        if(i >= node->children.size()) break;
    } while(!node->children[i]->leaf && (node = node->children[i]));

    BTreeNode<T, B, Comparator>* leaf = new BTreeNode<T, B, Comparator>(true);
    leaf->keys.push_back(key);
    int indexBeforeInsert = binarySearch(
            node->children.begin(),node->children.end(), key,
            typename BTreeNode<T, B, Comparator>::BTreeNodeComparator{comp}
            );

    if(node->children.size() != node->keys.size()){
        if(indexBeforeInsert == 0){
            T toInsert = node->children.front()->keys.front();
            node->keys.emplace(node->keys.begin() + indexBeforeInsert, toInsert);
        } else {
            node->keys.emplace(node->keys.begin() + indexBeforeInsert - 1, key);
        }
    }

    node->children.emplace(node->children.begin() + indexBeforeInsert, leaf);

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

    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;

    tree.insert(2);

    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;

    tree.insert(1);

    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;

    tree.insert(10);
    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;
    tree.insert(14);
    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;
    tree.insert(25);
    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;
    tree.insert(22);
    tree.insert(4);

    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;

    tree.remove(2);
    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;
    tree.remove(3);
    std::cout << "B-Tree:" << std::endl;
    tree.print();
    std::cout << std::endl;

    /*
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
    */
    return 0;
}