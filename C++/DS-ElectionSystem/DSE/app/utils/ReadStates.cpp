#include "ReadStates.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

ReadStates::ReadStates(const std::string& statesFilePath) {
    std::ifstream file(statesFilePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << statesFilePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string part;
        std::string state;
        int electors;

        if (std::getline(iss, part, ',')) {
            state = part;
        }
        if (std::getline(iss, part, ',')) {
            electors = std::stoi(part);
        }

        statesMap[state] = electors;
    }

    file.close();
}

const std::map<std::string, int>& ReadStates::getStatesMap() const {
    return statesMap;
}