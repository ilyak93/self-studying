#include <iostream>
#include <memory>

// Beverage interface.
class Beverage {
public:
    virtual void drink() = 0;
    virtual ~Beverage() = default;
};

// Drinks which can be decorated.
class Coffee : public Beverage {
public:
    virtual void drink() override {
        std::cout << "Drinking Coffee";
    }
};

class Soda : public Beverage {
public:
    virtual void drink() override {
        std::cout << "Drinking Soda";
    }
};

class BeverageDecorator : public Beverage {
public:
    BeverageDecorator() = delete;
    BeverageDecorator(std::unique_ptr<Beverage> component_)
            : component(std::move(component_))
    {}

    virtual void drink() = 0;

protected:
    void callComponentDrink() {
        if (component) {
            component->drink();
        }
    }

private:
    std::unique_ptr<Beverage> component;
};

class Milk : public BeverageDecorator {
public:
    Milk(std::unique_ptr<Beverage> component_, float percentage_)
            : BeverageDecorator(std::move(component_))
            , percentage(percentage_)
    {
    }

    virtual void drink() override {
        callComponentDrink();
        std::cout << ", with milk of richness " << percentage << "%";
    }

private:

    float percentage;
};

class IceCubes : public BeverageDecorator {
public:
    IceCubes(std::unique_ptr<Beverage> component_, int count_)
            : BeverageDecorator(std::move(component_))
            , count(count_)
    {
    }

    virtual void drink() override {
        callComponentDrink();
        std::cout << ", with " << count << " ice cubes";
    }

private:

    int count;
};

class Sugar : public BeverageDecorator {
public:
    Sugar(std::unique_ptr<Beverage> component_, int spoons_)
            : BeverageDecorator(std::move(component_))
            , spoons(spoons_)
    {
    }

    virtual void drink() override {
        callComponentDrink();
        std::cout << ", with " << spoons << " spoons of sugar";
    }

private:

    int spoons = 1;
};

int main() {

    std::unique_ptr<Beverage> soda = std::make_unique<Soda>();
    soda = std::make_unique<IceCubes>(std::move(soda), 3);
    soda = std::make_unique<Sugar>(std::move(soda), 1);

    soda->drink();
    std::cout << std::endl;

    std::unique_ptr<Beverage> coffee = std::make_unique<Coffee>();
    coffee = std::make_unique<IceCubes>(std::move(coffee), 16);
    coffee = std::make_unique<Milk>(std::move(coffee), 3.);
    coffee = std::make_unique<Sugar>(std::move(coffee), 2);

    coffee->drink();

    return 0;
}