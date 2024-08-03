#include <iostream>
#include <memory>

class View { // AbstractClass
public:
    // defines abstract primitive operations that concrete subclasses define to implement steps of an algorithm.
    virtual void doDisplay() {}
    // implements a template method defining the skeleton of an algorithm. The template method calls primitive operations as well as operations defined in AbstractClass or those of other objects.
    void display() {
        setFocus();
        doDisplay();
        resetFocus();
    }
    virtual ~View() = default;
private:
    void setFocus() {
        std::cout << "View::setFocus\n";
    }
    void resetFocus() {
        std::cout << "View::resetFocus\n";
    }
};

class MyView : public View { // ConcreteClass
    // implements the primitive operations to carry out subclass-specific steps of the algorithm.
    void doDisplay() override {
        // render the view's contents
        std::cout << "MyView::doDisplay\n";
    }
};

int main() {
    // The smart pointers prevent memory leaks
    std::unique_ptr<View> myview = std::make_unique<MyView>();
    myview->display();
}