#include <iostream>
#include <vector>
#include <memory>

// Forward declarations
class Wheel;
class Engine;
class Body;
class Car;

// Visitor interface
class CarElementVisitor {
public:
    virtual void visit(Wheel& wheel) = 0;
    virtual void visit(Engine& engine) = 0;
    virtual void visit(Body& body) = 0;
    virtual void visit(Car& car) = 0;
    virtual ~CarElementVisitor() = default;
};

// Element interface
class CarElement {
public:
    virtual void accept(CarElementVisitor& visitor) = 0;
    virtual ~CarElement() = default;
};

// Concrete elements
class Wheel : public CarElement {
public:
    explicit Wheel(const std::string& name) : name(name) {}
    void accept(CarElementVisitor& visitor) override {
        visitor.visit(*this);
    }
    std::string getName() const { return name; }
private:
    std::string name;
};

class Engine : public CarElement {
public:
    void accept(CarElementVisitor& visitor) override {
        visitor.visit(*this);
    }
};

class Body : public CarElement {
public:
    void accept(CarElementVisitor& visitor) override {
        visitor.visit(*this);
    }
};

class Car : public CarElement {
public:
    Car() {
        elements.push_back(std::make_unique<Wheel>("front left"));
        elements.push_back(std::make_unique<Wheel>("front right"));
        elements.push_back(std::make_unique<Wheel>("back left"));
        elements.push_back(std::make_unique<Wheel>("back right"));
        elements.push_back(std::make_unique<Body>());
        elements.push_back(std::make_unique<Engine>());
    }
    void accept(CarElementVisitor& visitor) override {
        for (auto& elem : elements) {
            elem->accept(visitor);
        }
        visitor.visit(*this);
    }
private:
    std::vector<std::unique_ptr<CarElement>> elements;
};

// Concrete visitors
class CarElementPrintVisitor : public CarElementVisitor {
public:
    void visit(Wheel& wheel) override {
        std::cout << "Visiting " << wheel.getName() << " wheel\n";
    }
    void visit(Engine& engine) override {
        std::cout << "Visiting engine\n";
    }
    void visit(Body& body) override {
        std::cout << "Visiting body\n";
    }
    void visit(Car& car) override {
        std::cout << "Visiting car\n";
    }
};

class CarElementDoVisitor : public CarElementVisitor {
public:
    void visit(Wheel& wheel) override {
        std::cout << "Kicking my " << wheel.getName() << " wheel\n";
    }
    void visit(Engine& engine) override {
        std::cout << "Starting my engine\n";
    }
    void visit(Body& body) override {
        std::cout << "Moving my body\n";
    }
    void visit(Car& car) override {
        std::cout << "Starting my car\n";
    }
};

int main() {
    Car car;
    CarElementPrintVisitor printVisitor;
    CarElementDoVisitor doVisitor;

    std::cout << "Print visitor:\n";
    car.accept(printVisitor);

    std::cout << "\nDo visitor:\n";
    car.accept(doVisitor);

    return 0;
}