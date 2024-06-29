#include "VotesCountKey.hpp"

// Constructor
VotesCountKey::VotesCountKey(const std::string& party, const std::string& state)
    : party(party), state(state) {}

// Setter for party
VotesCountKey& VotesCountKey::setParty(const std::string& party) {
    this->party = party;
    return *this;
}

// Setter for state
VotesCountKey& VotesCountKey::setState(const std::string& state) {
    this->state = state;
    return *this;
}

// Getter for party
std::string VotesCountKey::getParty() const {
    return party;
}

// Getter for state
std::string VotesCountKey::getState() const {
    return state;
}

// Custom hash function
size_t VotesCountKey::hash() const {
    return std::hash<std::string>()(state) ^ (std::hash<std::string>()(party) << 1);
}

// Equality operator
bool VotesCountKey::operator==(const VotesCountKey& other) const {
    return party == other.party && state == other.state;
}