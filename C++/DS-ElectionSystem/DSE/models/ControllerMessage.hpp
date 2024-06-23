#ifndef CONTROLLERMESSAGE_HPP
#define CONTROLLERMESSAGE_HPP

#include <string>

class ControllerMessage {
private:
    std::string message;

public:
    // Default constructor
    ControllerMessage();

    // Parameterized constructor
    ControllerMessage(const std::string& message);

    // Setter method
    ControllerMessage& setMessage(const std::string& message);

    // Getter method
    std::string getMessage() const;
};

#endif // CONTROLLERMESSAGE_HPP