#ifndef ONE_TIME_USE_ELEMENT_HPP
#define ONE_TIME_USE_ELEMENT_HPP

template <typename T>
class OneTimeUseElement {
private:
    T element;
    bool used;

public:
    OneTimeUseElement(T elem) : element(std::move(elem)), used(false) {}

    T& getElement() {
        return element;
    }

    bool isUsed() const {
        return used;
    }

    void use() {
        used = true;
    }
};

#endif // ONE_TIME_USE_ELEMENT_HPP