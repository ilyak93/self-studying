#ifndef READSTATES_HPP
#define READSTATES_HPP

#include <string>
#include <map>

class ReadStates {
private:
    std::map<std::string, int> statesMap;

public:
    ReadStates(const std::string& statesFilePath);
    const std::map<std::string, int>& getStatesMap() const;
};

#endif // READSTATES_HPP