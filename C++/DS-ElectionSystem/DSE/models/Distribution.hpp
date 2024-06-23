#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP

#include <string>

class Distribution {
private:
    std::string party;
    int electors;

public:
    // Constructor
    Distribution(const std::string& party, int electors);

    // Getters
    std::string getParty() const;
    int getElectors() const;
};

#endif // DISTRIBUTION_HPP