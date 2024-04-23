#include "stack_exceptions.hpp"

template<typename T>
class Stack {
public:
    Stack() : top(nullptr) {}
    ~Stack() {
        clear();
    }

    void push(const T& value) {
        Node* newNode = new Node(value, top);
        top = newNode;
    }

    T pop() {
        if (isEmpty()) {
            throw StackUnderflowException();
        }
        Node* oldTop = top;
        T data = oldTop->data;
        top = oldTop->next;
        delete oldTop;
        return data;
    }

    T peek() const {
        if (isEmpty()) {
            throw StackUnderflowException();
        }
        return top->data;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    void clear() {
        while (!isEmpty()) {
            Node* oldTop = top;
            top = oldTop->next;
            delete oldTop;
        }
    }

private:
    struct Node {
        T data;
        Node* next;
        Node(const T& data, Node* next) : data(data), next(next) {}
    };

    Node* top;
};