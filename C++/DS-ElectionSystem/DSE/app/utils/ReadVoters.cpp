#include "ReadVoters.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

ReadVoters::ReadVoters(const std::string& votersFilePath) {
    std::ifstream file(votersFilePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << votersFilePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string part;
        int voterId;
        std::string originState;

        if (std::getline(iss, part, ',')) {
            voterId = std::stoi(part);
        }
        if (std::getline(iss, part, ',')) {
            originState = part;
        }

        votersMap[voterId] = originState;
    }

    file.close();
}

const std::map<int, std::string>& ReadVoters::getVotersMap() const {
    return votersMap;
}