#ifndef VOTESCOUNT_HPP
#define VOTESCOUNT_HPP

#include <string>
#include "Server.hpp"

class VotesCount {
private:
    std::string party;
    int count;
    std::string state;

public:
    // Constructors
    VotesCount(const std::string& party);
    VotesCount(const std::string& party, int count);
    VotesCount(const std::string& party, int count, const std::string& state);

    // Methods
    VotesCount& add(int moreCount);
    VotesCount& add();

    // Getters
    std::string getParty() const;
    int getCount() const;
    std::string getState() const;
};

#endif // VOTESCOUNT_HPP