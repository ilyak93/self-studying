#include <iostream>
#include <string>

struct Circle {
    void Resize(float factor) { radius *= factor; }

    std::string GetName() const {
        return std::string("A circle of radius ") + std::to_string(radius);
    }

    float radius = 10.0f;
};

template <typename T>
struct ColoredShape : public T {
    ColoredShape(const std::string& color) : color(color) {}

    std::string GetName() const {
        return T::GetName() + " which is colored " + color;
    }

    std::string color;
};

int main() {
    ColoredShape<Circle> red_circle("red");
    std::cout << red_circle.GetName() << std::endl;
    red_circle.Resize(1.5f);
    std::cout << red_circle.GetName() << std::endl;
}