#ifndef READVOTERS_HPP
#define READVOTERS_HPP

#include <string>
#include <map>

class ReadVoters {
private:
    std::map<int, std::string> votersMap;

public:
    ReadVoters(const std::string& votersFilePath);
    const std::map<int, std::string>& getVotersMap() const;
};

#endif // READVOTERS_HPP