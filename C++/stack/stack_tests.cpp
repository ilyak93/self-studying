#include <iostream>
#include <string>

void assertTrue(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << testName << ": PASS\n";
    } else {
        std::cout << testName << ": FAIL\n";
    }
}

#include "Stack.hpp"  // Assume Stack is implemented in this header

// Test push and peek
void testPushAndPeek() {
    Stack<int> stack;
    stack.push(42);
    assertTrue(stack.peek() == 42, "testPushAndPeek - Peek after push");
}

// Test pop
void testPop() {
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    int value = stack.pop();
    assertTrue(value == 20, "testPop - Pop the top element");
    assertTrue(stack.peek() == 10, "testPop - Peek after pop");
}

// Test isEmpty
void testIsEmpty() {
    Stack<int> stack;
    assertTrue(stack.isEmpty(), "testIsEmpty - Initially empty");

    stack.push(1);
    assertTrue(!stack.isEmpty(), "testIsEmpty - Not empty after push");

    stack.pop();
    assertTrue(stack.isEmpty(), "testIsEmpty - Empty after pop");
}

// Test clear
void testClear() {
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);
    stack.clear();
    assertTrue(stack.isEmpty(), "testClear - Stack should be empty after clear");
}

// Test exceptions
void testExceptions() {
    Stack<int> stack;
    try {
        stack.pop();
        assertTrue(false, "testExceptions - Underflow not thrown on pop");
    } catch (const StackUnderflowException& e) {
        assertTrue(true, "testExceptions - Underflow thrown on pop");
    }

    try {
        stack.peek();
        assertTrue(false, "testExceptions - Underflow not thrown on peek");
    } catch (const StackUnderflowException& e) {
        assertTrue(true, "testExceptions - Underflow thrown on peek");
    }
}

// Run all tests
int main() {
    testPushAndPeek();
    testPop();
    testIsEmpty();
    testClear();
    testExceptions();
    return 0;
}