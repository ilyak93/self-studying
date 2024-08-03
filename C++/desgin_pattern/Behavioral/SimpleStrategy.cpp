#include <iostream>
#include <memory>

// Strategy interface
class IBrakeBehavior {
public:
    virtual ~IBrakeBehavior() = default;
    virtual void brake() = 0;
};

// Concrete strategies
class Brake : public IBrakeBehavior {
public:
    void brake() override {
        std::cout << "Simple Brake applied" << std::endl;
    }
};

class BrakeWithABS : public IBrakeBehavior {
public:
    void brake() override {
        std::cout << "Brake with ABS applied" << std::endl;
    }
};

// Context
class Car {
private:
    std::unique_ptr<IBrakeBehavior> brakeBehavior;

public:
    Car(std::unique_ptr<IBrakeBehavior> behavior) : brakeBehavior(std::move(behavior)) {}

    void applyBrake() {
        brakeBehavior->brake();
    }

    void setBrakeBehavior(std::unique_ptr<IBrakeBehavior> behavior) {
            brakeBehavior = std::move(behavior);
    }
};

// Concrete context classes
class Sedan : public Car {
public:
    Sedan() : Car(std::make_unique<Brake>()) {}
};

class SUV : public Car {
public:
    SUV() : Car(std::make_unique<BrakeWithABS>()) {}
};

int main() {
    Sedan sedanCar;
    sedanCar.applyBrake();  // This will invoke class "Brake"

    SUV suvCar;
    suvCar.applyBrake();    // This will invoke class "BrakeWithABS"

    // Set brake behavior dynamically
    suvCar.setBrakeBehavior(std::make_unique<Brake>());
    suvCar.applyBrake();    // This will now invoke class "Brake"

    return 0;
}