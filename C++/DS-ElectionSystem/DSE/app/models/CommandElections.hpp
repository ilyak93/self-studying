
#ifndef COMMANDELECTIONS_HPP
#define COMMANDELECTIONS_HPP

#include <string>

class CommandElections {
private:
    std::string command;

public:
    // Constructor
    CommandElections(const std::string& command);

    // Setter method
    CommandElections& setCommand(const std::string& command);

    // Getter method
    std::string getCommand() const;
};

#endif // COMMANDELECTIONS_HPP