#include "Distribution.hpp"

// Constructor
Distribution::Distribution(const std::string& party, int electors)
    : party(party), electors(electors) {}

// Getter for party
std::string Distribution::getParty() const {
    return party;
}

// Getter for electors
int Distribution::getElectors() const {
    return electors;
}