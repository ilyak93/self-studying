#include <iostream>
#include <string>
#include <list>
#include <memory>

typedef double Currency;

// Base interface for all components
class Equipment {
public:
    virtual const std::string& getName() const = 0;
    virtual void setName(const std::string& name) = 0;
    virtual Currency getNetPrice() const = 0;
    virtual void setNetPrice(Currency netPrice) = 0;
    virtual ~Equipment() = default;
};

// Interface for composite objects
class CompositeEquipment : public Equipment {
public:
    virtual void add(std::shared_ptr<Equipment> equipment) = 0;
    virtual void remove(std::shared_ptr<Equipment> equipment) = 0;
};

// Leaf class
class FloppyDisk : public Equipment {
public:
    FloppyDisk(const std::string& name) : name(name), netPrice(0) {}

    const std::string& getName() const override { return name; }
    void setName(const std::string& name_) override { name = name_; }
    Currency getNetPrice() const override { return netPrice; }
    void setNetPrice(Currency netPrice_) override { netPrice = netPrice_; }

private:
    std::string name;
    Currency netPrice;
};

// Composite class
class Chassis : public CompositeEquipment {
public:
    Chassis(const std::string& name) : name(name), netPrice(0) {}

    const std::string& getName() const override { return name; }
    void setName(const std::string& name_) override { name = name_; }
    Currency getNetPrice() const override {
        Currency total = netPrice;
        for (const auto& item : equipment) {
            total += item->getNetPrice();
        }
        return total;
    }
    void setNetPrice(Currency netPrice_) override { netPrice = netPrice_; }

    void add(std::shared_ptr<Equipment> equipment_) override {
        equipment.push_back(equipment_);
    }
    void remove(std::shared_ptr<Equipment> equipment_) override {
        equipment.remove(equipment_);
    }

private:
    std::string name;
    Currency netPrice;
    std::list<std::shared_ptr<Equipment>> equipment;
};

int main() {
    auto fd1 = std::make_shared<FloppyDisk>("3.5in Floppy");
    fd1->setNetPrice(19.99);
    std::cout << fd1->getName() << ": netPrice=" << fd1->getNetPrice() << '\n';

    auto fd2 = std::make_shared<FloppyDisk>("5.25in Floppy");
    fd2->setNetPrice(29.99);
    std::cout << fd2->getName() << ": netPrice=" << fd2->getNetPrice() << '\n';

    auto chassis = std::make_shared<Chassis>("PC Chassis");
    chassis->setNetPrice(39.99);
    chassis->add(fd1);
    chassis->add(fd2);
    std::cout << chassis->getName() << ": netPrice=" << chassis->getNetPrice() << '\n';

    // This line would now cause a compilation error
    // fd1->add(fd2);  // Error: FloppyDisk has no member named 'add'
}