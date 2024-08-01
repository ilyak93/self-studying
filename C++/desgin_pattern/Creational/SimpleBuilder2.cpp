#include <string>

// Represents a product created by the builder.
class Bicycle {
public:
    Bicycle(std::string make, std::string model, std::string colour, int height)
            : make_(make), model_(model), colour_(colour), height_(height) {}

    std::string getMake() const { return make_; }
    void setMake(const std::string& make) { make_ = make; }

    std::string getModel() const { return model_; }
    void setModel(const std::string& model) { model_ = model; }

    int getHeight() const { return height_; }
    void setHeight(int height) { height_ = height; }

    std::string getColour() const { return colour_; }
    void setColour(const std::string& colour) { colour_ = colour; }

private:
    std::string make_;
    std::string model_;
    int height_;
    std::string colour_;
};

// The builder abstraction.
class IBicycleBuilder {
public:
    virtual ~IBicycleBuilder() = default;
    virtual Bicycle* GetResult() = 0;

    virtual void setColour(const std::string& colour) = 0;
    virtual void setHeight(int height) = 0;
};

// Concrete builder implementation.
class GTBuilder : public IBicycleBuilder {
public:
    Bicycle* GetResult() override {
        if (height_ == 29) {
            return new Bicycle("GT", "Avalanche", colour_, height_);
        }
        return nullptr;
    }

    void setColour(const std::string& colour) override { colour_ = colour; }
    void setHeight(int height) override { height_ = height; }

private:
    std::string colour_;
    int height_;
};

// The director.
class MountainBikeBuildDirector {
public:
    MountainBikeBuildDirector(IBicycleBuilder* builder) : builder_(builder) {}

    void Construct() {
        builder_->setColour("Red");
        builder_->setHeight(29);
    }

    Bicycle* GetResult() {
        return builder_->GetResult();
    }

private:
    IBicycleBuilder* builder_;
};

class Client {
public:
    void DoSomethingWithBicycles() {
        GTBuilder* gtBuilder = new GTBuilder();
        MountainBikeBuildDirector director(gtBuilder);
        // Director controls the stepwise creation of product and returns the result.
        director.Construct();
        Bicycle* myMountainBike = director.GetResult();

        // Don't forget to delete the dynamically allocated objects
        delete gtBuilder;
        delete myMountainBike;
    }
};