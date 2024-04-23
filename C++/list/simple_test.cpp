#include "list.hpp"

int main() {
    LinkedList<int> list;
    list.insertAtBeginning(3);
    list.insertAtBeginning(2);
    list.insertAtBeginning(1);

    for (LinkedList<int>::Iterator it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << std::endl;
    }

    list.deleteValue(2);
    list.print();

    return 0;
}