#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <stdexcept>

typedef double Currency;

// declares the interface for objects in the composition.
class Equipment { // Component
public:
    // implements default behavior for the interface common to all classes, as appropriate.
    virtual const std::string& getName() {
        return name;
    }
    virtual void setName(const std::string& name_) {
        name = name_;
    }
    virtual Currency getNetPrice() {
        return netPrice;
    }
    virtual void setNetPrice(Currency netPrice_) {
        netPrice = netPrice_;
    }
    // declares an interface for accessing and managing its child components.
    virtual void add(std::shared_ptr<Equipment>) = 0;
    virtual void remove(std::shared_ptr<Equipment>) = 0;
    virtual ~Equipment() = default;
protected:
    Equipment() :name(""), netPrice(0) {}
    Equipment(const std::string& name_) :name(name_), netPrice(0) {}
private:
    std::string name;
    Currency netPrice;
};

// defines behavior for components having children.
class CompositeEquipment : public Equipment { // Composite
public:
    // implements child-related operations in the Component interface.
    virtual Currency getNetPrice() override {
        Currency total = Equipment::getNetPrice();
        for (const auto& i:equipment) {
            total += i->getNetPrice();
        }
        return total;
    }
    virtual void add(std::shared_ptr<Equipment> equipment_) override {
        equipment.push_front(equipment_.get());
    }
    virtual void remove(std::shared_ptr<Equipment> equipment_) override {
        equipment.remove(equipment_.get());
    }
protected:
    CompositeEquipment() :equipment() {}
    CompositeEquipment(const std::string& name_) :equipment() {
        setName(name_);
    }
private:
    // stores child components.
    std::list<Equipment*> equipment;
};

// represents leaf objects in the composition.
class FloppyDisk : public Equipment { // Leaf
public:
    FloppyDisk(const std::string& name_) {
        setName(name_);
    }
    // A leaf has no children.
    void add(std::shared_ptr<Equipment>) override {
        throw std::runtime_error("FloppyDisk::add");
    }
    void remove(std::shared_ptr<Equipment>) override {
        throw std::runtime_error("FloppyDisk::remove");
    }
};

class Chassis : public CompositeEquipment {
public:
    Chassis(const std::string& name_) {
        setName(name_);
    }
};

int main() {
    // The smart pointers prevent memory leaks.
    std::shared_ptr<FloppyDisk> fd1 = std::make_shared<FloppyDisk>("3.5in Floppy");
    fd1->setNetPrice(19.99);
    std::cout << fd1->getName() << ": netPrice=" << fd1->getNetPrice() << '\n';

    std::shared_ptr<FloppyDisk> fd2 = std::make_shared<FloppyDisk>("5.25in Floppy");
    fd2->setNetPrice(29.99);
    std::cout << fd2->getName() << ": netPrice=" << fd2->getNetPrice() << '\n';

    std::unique_ptr<Chassis> ch = std::make_unique<Chassis>("PC Chassis");
    ch->setNetPrice(39.99);
    ch->add(fd1);
    ch->add(fd2);
    std::cout << ch->getName() << ": netPrice=" << ch->getNetPrice() << '\n';
    fd2->add(fd1);
}