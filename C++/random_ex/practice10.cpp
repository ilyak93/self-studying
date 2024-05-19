#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node{
    Node* next;
    Node* prev;
    int value;
    int key;
    Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
    Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache{

protected:
    map<int,Node*> mp; //map the key to the node in the linked list
    int cp;  //capacity
    Node* tail; // double linked list tail pointer
    Node* head; // double linked list head pointer
    virtual void set(int, int) = 0; //set function
    virtual int get(int) = 0; //get function

};

class LRUCache : public Cache {
public:
    LRUCache(int capacity) {
        cp = capacity;
        head = nullptr;
        tail = nullptr;
    }
    void set(int key, int value) override {
        auto it = mp.find(key);

        if (it != mp.end()) {
            it->second->value = value;
            if(it->second->prev)
                it->second->prev->next = it->second->next;
            if (it->second->next) it->second->next->prev = it->second->prev;
            else tail = it->second->prev;

            it->second->next = head;
            it->second->prev = nullptr;
            head->prev = it->second;
            head = it->second;
        } else {
            Node* node = new Node(key, value);

            if (mp.size() == cp) {
                mp.erase(tail->key);
                tail = tail->prev;
                if(tail) {
                    delete tail->next;
                    tail->next = nullptr;
                }
            }

            node->next = head;
            if (head && head->prev) head->prev = node;
            head = node;
            if (!tail) tail = head;

            mp[key] = node;
        }
    }

    int get(int key) override {
        auto it = mp.find(key);
        if (it != mp.end())
        {
            set(key, it->second->value);
            return it->second->value;
        }

        return -1;
    }
};

int main() {
    int n, capacity,i;
    cin >> n >> capacity;
    LRUCache l(capacity);
    for(i=0;i<n;i++) {
        string command;
        cin >> command;
        if(command == "get") {
            int key;
            cin >> key;
            cout << l.get(key) << endl;
        }
        else if(command == "set") {
            int key, value;
            cin >> key >> value;
            l.set(key,value);
        }
    }
    return 0;
}

/*
 * exmaple input
8 4
set 4 2
set 2 7
get 2
set 1 8
set 5 9
set 6 15
get 4
get 5
 */