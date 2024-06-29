
#ifndef VOTE_HPP
#define VOTE_HPP

#include <string>
#include <ctime>
#include <iostream>

class Vote {
private:
    int clientId;
    std::string party;
    std::string originState;
    std::string currentState;
    long timeStamp;
    bool counted;
    bool broadcastedInState;
    bool valid;
    int serverId;

public:
    Vote(int clientId, const std::string& party);
    Vote(int clientId, const std::string& party, const std::string& originState, const std::string& currentState);
    Vote(int clientId, const std::string& party, const std::string& originState, const std::string& currentState, long timeStamp);

    // Setter methods
    Vote& setClientId(int clientId);
    Vote& setParty(const std::string& party);
    Vote& setOriginState(const std::string& originState);
    Vote& setCurrentState(const std::string& currentState);
    Vote& setTimeStamp(long timeStamp);

    // Getter methods
    int getClientId() const;
    std::string getParty() const;
    std::string getOriginState() const;
    std::string getCurrentState() const;
    long getTimeStamp() const;

    // Other methods
    bool isCounted() const;
    void count();
    bool isBroadcastedInState() const;
    void broadcastInState();
    bool isValid() const;
    int getServerId() const;
    std::string toString() const;
};

#endif // VOTE_HPP