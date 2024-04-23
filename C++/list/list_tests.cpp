#include "list.hpp"
#include <cassert>
#include <iostream>

// Test the functionality of inserting at the beginning
void testInsertAtBeginning() {
    LinkedList<int> list;
    list.insertAtBeginning(1);
    list.insertAtBeginning(2);
    list.insertAtBeginning(3);

    // Check the order of elements is as expected
    auto it = list.begin();
    assert(*it == 3 && "First element should be 3");
    ++it;
    assert(*it == 2 && "Second element should be 2");
    ++it;
    assert(*it == 1 && "Third element should be 1");
    ++it;
    assert(it == list.end() && "Iterator should be at the end");

    std::cout << "testInsertAtBeginning passed.\n";
}

// Test deleting elements
void testDeleteValue() {
    LinkedList<int> list;
    list.insertAtBeginning(1);
    list.insertAtBeginning(2);
    list.insertAtBeginning(3);

    // Delete the middle element
    list.deleteValue(2);
    auto it = list.begin();
    assert(*it == 3 && "First element should still be 3 after deletion");
    ++it;
    assert(*it == 1 && "Second element should be 1 after deletion");
    ++it;
    assert(it == list.end() && "Iterator should be at the end after deletion");

    // Attempt to delete a non-existing element
    list.deleteValue(99);
    assert(list.begin() != list.end() && "List should not be empty after trying to delete non-existing element");

    auto it2 = list.begin();
    assert(*it2 == 3 && "First element should still be 3 after deletion");
    ++it2;
    assert(*it2 == 1 && "Second element should be 1 after deletion");
    ++it2;
    assert(it2 == list.end() && "Iterator should be at the end after deletion");

    std::cout << "testDeleteValue passed.\n";
}

// Test print functionality by checking console output manually
void testPrint() {
    LinkedList<int> list;
    list.insertAtBeginning(1);
    list.insertAtBeginning(2);
    list.insertAtBeginning(3);

    std::cout << "List should print: 3 -> 2 -> 1 -> null\n";
    list.print();

    std::cout << "testPrint passed (manual verification required).\n";
}

// Test the iterator functionality separately
void testIterator() {
    LinkedList<int> list;
    list.insertAtBeginning(1);
    list.insertAtBeginning(2);
    list.insertAtBeginning(3);

    int expectedValue = 3;
    for (auto value : list) {
        assert(value == expectedValue-- && "Iterator did not iterate in the correct order");
    }
    assert(expectedValue == 0 && "Iterator did not iterate through all elements");

    std::cout << "testIterator passed.\n";
}

int main() {
    testInsertAtBeginning();
    testDeleteValue();
    testPrint();
    testIterator();
    return 0;
}