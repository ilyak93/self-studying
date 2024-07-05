#ifndef READSTATES_HPP
#define READSTATES_HPP

#include <string>
#include <unordered_map>

class ReadStates {
private:
    std::unordered_map<std::string, int> statesMap;

public:
    ReadStates(const std::string& statesFilePath);
    const std::unordered_map<std::string, int>& getStatesMap() const;
};

#endif // READSTATES_HPP