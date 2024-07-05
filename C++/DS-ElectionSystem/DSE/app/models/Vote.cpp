#include "Vote.hpp"
#include"Server.hpp"

// Constructor with clientId and party
Vote::Vote(int clientId, const std::string& party) 
    : clientId(clientId), party(party), counted(false), broadcastedInState(false), serverId(Server::serverId) {
    originState = Server::clientIdToOriginState[clientId];
    currentState = Server::state;
    timeStamp = std::time(0);
    valid = !originState.empty();
}

// Constructor with clientId, party, originState, and currentState
Vote::Vote(int clientId, const std::string& party, const std::string& originState, const std::string& currentState)
    : clientId(clientId), party(party), originState(originState), currentState(currentState), counted(false), broadcastedInState(false), serverId(Server::serverId) {
    timeStamp = std::time(0);
    valid = !originState.empty();
}

// Constructor with clientId, party, originState, currentState, and timeStamp
Vote::Vote(int clientId, const std::string& party, const std::string& originState, const std::string& currentState, long timeStamp)
    : clientId(clientId), party(party), originState(originState), currentState(currentState), timeStamp(timeStamp), counted(false), broadcastedInState(false), serverId(Server::serverId) {
    valid = !originState.empty();
}

// Setter methods
Vote& Vote::setClientId(int clientId) {
    this->clientId = clientId;
    return *this;
}

Vote& Vote::setParty(const std::string& party) {
    this->party = party;
    return *this;
}

Vote& Vote::setOriginState(const std::string& originState) {
    this->originState = originState;
    return *this;
}

Vote& Vote::setCurrentState(const std::string& currentState) {
    this->currentState = currentState;
    return *this;
}

Vote& Vote::setTimeStamp(long timeStamp) {
    this->timeStamp = timeStamp;
    return *this;
}

// Getter methods
int Vote::getClientId() const {
    return clientId;
}

std::string Vote::getParty() const {
    return party;
}

std::string Vote::getOriginState() const {
    return originState;
}

std::string Vote::getCurrentState() const {
    return currentState;
}

long Vote::getTimeStamp() const {
    return timeStamp;
}

// Other methods
bool Vote::isCounted() const {
    return counted;
}

void Vote::count() {
    counted = true;
}

bool Vote::isBroadcastedInState() const {
    return broadcastedInState;
}

void Vote::broadcastInState() {
    broadcastedInState = true;
}

bool Vote::isValid() const {
    return valid;
}

int Vote::getServerId() const {
    return serverId;
}

std::string Vote::toString() const {
    return std::to_string(clientId) + "," + party + "," + originState + "," + currentState + "," + std::to_string(timeStamp);
}