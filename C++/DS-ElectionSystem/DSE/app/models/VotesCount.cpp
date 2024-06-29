
#include "VotesCount.hpp"

// Constructor with party
VotesCount::VotesCount(const std::string& party)
    : party(party), count(1), state(Server::state) {}

// Constructor with party and count
VotesCount::VotesCount(const std::string& party, int count)
    : party(party), count(count), state(Server::state) {}

// Constructor with party, count, and state
VotesCount::VotesCount(const std::string& party, int count, const std::string& state)
    : party(party), count(count), state(state) {}

// Add more count
VotesCount& VotesCount::add(int moreCount) {
    this->count += moreCount;
    return *this;
}

// Increment count by one
VotesCount& VotesCount::add() {
    this->count++;
    return *this;
}

// Get party
std::string VotesCount::getParty() const {
    return party;
}

// Get count
int VotesCount::getCount() const {
    return count;
}

// Get state
std::string VotesCount::getState() const {
    return state;
}