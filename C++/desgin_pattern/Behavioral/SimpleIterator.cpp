#include <iostream>
#include <stdexcept>
#include <initializer_list>

class Vector {
public:
    using iterator = double*;
    iterator begin() { return elem; }
    iterator end() { return elem + sz; }

    Vector(std::initializer_list<double> lst) :elem(nullptr), sz(0) {
        sz = lst.size();
        elem = new double[sz];
        double* p = elem;
        for (auto i = lst.begin(); i != lst.end(); ++i, ++p) {
            *p = *i;
        }
    }
    ~Vector() { delete[] elem; }
    int size() const { return sz; }
    double& operator[](int n) {
        if (n < 0 || n >= sz) throw std::out_of_range("Vector::operator[]");
        return elem[n];
    }
    Vector(const Vector&) = delete; // rule of three
    Vector& operator=(const Vector&) = delete;
private:
    double* elem;
    int sz;
};

int main() {
    Vector v = {1.1*1.1, 2.2*2.2};

    for (const auto& x : v) {
        std::cout << x << '\n';
    }
    for (auto i = v.begin(); i != v.end(); ++i) {
        std::cout << *i << '\n';
    }
    for (auto i = 0; i <= v.size(); ++i) {
        std::cout << v[i] << '\n';
    }
}