#include <iostream>
#include <memory>

// Forward declaration
class Context;

// State interface
class State {
public:
    virtual ~State() = default;
    virtual void handle(Context& context) = 0;
};

// Context class
class Context {
private:
    std::unique_ptr<State> current_state;

public:
    Context(std::unique_ptr<State> initial_state) : current_state(std::move(initial_state)) {}

    void setState(std::unique_ptr<State> new_state) {
        current_state = std::move(new_state);
    }

    void request() {
        current_state->handle(*this);
    }
};

// ConcreteStateA
class ConcreteStateA : public State {
public:
    void handle(Context& context) override;
};

// ConcreteStateB
class ConcreteStateB : public State {
public:
    void handle(Context& context) override;
};

// Implementation of ConcreteStateA's handle method
void ConcreteStateA::handle(Context& context) {
    std::cout << "ConcreteStateA handles the request." << std::endl;
    std::cout << "Changing to ConcreteStateB." << std::endl;
    context.setState(std::make_unique<ConcreteStateB>());
}

// Implementation of ConcreteStateB's handle method
void ConcreteStateB::handle(Context& context) {
    std::cout << "ConcreteStateB handles the request." << std::endl;
    std::cout << "Changing to ConcreteStateA." << std::endl;
    context.setState(std::make_unique<ConcreteStateA>());
}

int main() {
    // Start with ConcreteStateA
    auto context = std::make_unique<Context>(std::make_unique<ConcreteStateA>());

    // Make some requests
    context->request();  // This will use ConcreteStateA and change to ConcreteStateB
    context->request();  // This will use ConcreteStateB and change to ConcreteStateA
    context->request();  // This will use ConcreteStateA and change to ConcreteStateB

    return 0;
}