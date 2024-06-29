#include "CommandElections.hpp"

// Constructor
CommandElections::CommandElections(const std::string& command)
    : command(command) {}

// Setter for command
CommandElections& CommandElections::setCommand(const std::string& command) {
    this->command = command;
    return *this;
}

// Getter for command
std::string CommandElections::getCommand() const {
    return command;
}