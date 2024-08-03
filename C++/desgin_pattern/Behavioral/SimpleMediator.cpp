#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Forward declaration
class Mediator;

// Colleague interface
class Colleague {
protected:
    Mediator* mediator;

public:
    Colleague(Mediator* m) : mediator(m) {}
    virtual void send(const std::string& message) = 0;
    virtual void receive(const std::string& message) = 0;
};

// Concrete Colleague classes
class ConcreteColleague1 : public Colleague {
public:
    ConcreteColleague1(Mediator* m) : Colleague(m) {}
    void send(const std::string& message) override;
    void receive(const std::string& message) override {
        std::cout << "Colleague1 received: " << message << std::endl;
    }
};

class ConcreteColleague2 : public Colleague {
public:
    ConcreteColleague2(Mediator* m) : Colleague(m) {}
    void send(const std::string& message) override;
    void receive(const std::string& message) override {
        std::cout << "Colleague2 received: " << message << std::endl;
    }
};

// Mediator interface
class Mediator {
public:
    virtual void addColleague(std::shared_ptr<Colleague> colleague) = 0;
    virtual void distribute(const Colleague* sender, const std::string& message) = 0;
};

// Concrete Mediator
class ConcreteMediator : public Mediator {
private:
    std::vector<std::shared_ptr<Colleague>> colleagues;

public:
    void addColleague(std::shared_ptr<Colleague> colleague) override {
        colleagues.push_back(colleague);
    }

    void distribute(const Colleague* sender, const std::string& message) override {
        for (const auto& colleague : colleagues) {
            if (colleague.get() != sender) {
                colleague->receive(message);
            }
        }
    }
};

// Implementation of Colleague send methods
void ConcreteColleague1::send(const std::string& message) {
    std::cout << "Colleague1 sends: " << message << std::endl;
    mediator->distribute(this, message);
}

void ConcreteColleague2::send(const std::string& message) {
    std::cout << "Colleague2 sends: " << message << std::endl;
    mediator->distribute(this, message);
}

int main() {
    auto mediator = std::make_shared<ConcreteMediator>();

    auto colleague1 = std::make_shared<ConcreteColleague1>(mediator.get());
    auto colleague2 = std::make_shared<ConcreteColleague2>(mediator.get());

    mediator->addColleague(colleague1);
    mediator->addColleague(colleague2);

    colleague1->send("Hello from Colleague1");
    colleague2->send("Hi from Colleague2");

    return 0;
}