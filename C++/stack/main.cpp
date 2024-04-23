#include <iostream>
#include "stack.hpp"

int main() {
    Stack<int> intStack;

    try {
        intStack.push(10);
        intStack.push(20);
        intStack.push(30);

        std::cout << "Top element: " << intStack.peek() << std::endl;
        std::cout << "Popped element: " << intStack.pop() << std::endl;
        std::cout << "Next element: " << intStack.peek() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}