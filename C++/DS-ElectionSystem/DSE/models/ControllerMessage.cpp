#include "ControllerMessage.hpp"

// Default constructor
ControllerMessage::ControllerMessage()
    : message("") {}

// Parameterized constructor
ControllerMessage::ControllerMessage(const std::string& message)
    : message(message) {}

// Setter for message
ControllerMessage& ControllerMessage::setMessage(const std::string& message) {
    this->message = message;
    return *this;
}

// Getter for message
std::string ControllerMessage::getMessage() const {
    return message;
}