#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP

#include <string>

class Distribution {
private:
    std::string party;
    int electors;

public:
    Distribution(const std::string& party, int electors) : party(party), electors(electors) {}

    std::string getParty() const { return party; }
    int getElectors() const { return electors; }
};

#endif // DISTRIBUTION_HPP