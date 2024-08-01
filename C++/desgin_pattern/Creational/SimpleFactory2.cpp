#include <iostream>
#include <memory>


enum ProductId {MINE, YOURS};

// defines the interface of objects the factory method creates.
class Product {
public:
    virtual void print() = 0;
    virtual ~Product() = default;
};

// implements the Product interface.
class ConcreteProductMINE : public Product {
public:
    void print() {
        std::cout << "this=" << this << " print MINE\n";
    }
};

// implements the Product interface.
class ConcreteProductYOURS : public Product {
public:
    void print() {
        std::cout << "this=" << this << " print YOURS\n";
    }
};

// declares the factory method, which returns an object of type Product.
class Creator {
public:
    virtual std::unique_ptr<Product> create(ProductId id) {
        if (ProductId::MINE == id) return std::make_unique<ConcreteProductMINE>();
        if (ProductId::YOURS == id) return std::make_unique<ConcreteProductYOURS>();
        // repeat for remaining products...

        return nullptr;
    }
    virtual ~Creator() = default;
};

int main() {
    // The unique_ptr prevent memory leaks.
    std::unique_ptr<Creator> creator = std::make_unique<Creator>();
    std::unique_ptr<Product> product = creator->create(ProductId::MINE);
    product->print();

    product = creator->create(ProductId::YOURS);
    product->print();
}