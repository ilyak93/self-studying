// exceptions.hpp
#include <exception>

class StackOverflowException : public std::exception {
    const char* what() const throw() override {
        return "Data structure overflow";
    }
};

class StackUnderflowException : public std::exception {
    const char* what() const throw() override {
        return "Data structure underflow";
    }
};