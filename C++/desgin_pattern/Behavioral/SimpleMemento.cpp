#include <iostream>
#include <vector>
#include <memory>

class Originator; // Forward declaration

class Memento {
private:
    std::string state;
    Memento(const std::string& s) : state(s) {}

    friend class Originator; // Allow Originator to access private members

public:
    // Public getter is okay if we want to allow reading the state
    std::string getState() const { return state; }
};

class Originator {
private:
    std::string state;

public:
    void setState(const std::string& s) {
        std::cout << "Originator: Setting state to " << s << std::endl;
        state = s;
    }

    std::unique_ptr<Memento> saveToMemento() {
        std::cout << "Originator: Saving to Memento." << std::endl;
        return std::unique_ptr<Memento>(new Memento(state)); // Use new instead of make_unique
    }

    void restoreFromMemento(const Memento* m) {
        state = m->getState();
        std::cout << "Originator: State after restoring from Memento: " << state << std::endl;
    }
};

class Caretaker {
private:
    std::vector<std::unique_ptr<Memento>> savedStates;

public:
    void addMemento(std::unique_ptr<Memento> m) {
        savedStates.push_back(std::move(m));
    }

    const Memento* getMemento(int index) const {
        return savedStates[index].get();
    }
};

int main() {
    Originator originator;
    Caretaker caretaker;

    originator.setState("State1");
    originator.setState("State2");
    caretaker.addMemento(originator.saveToMemento());

    originator.setState("State3");
    caretaker.addMemento(originator.saveToMemento());

    originator.setState("State4");

    originator.restoreFromMemento(caretaker.getMemento(1));

    return 0;
}