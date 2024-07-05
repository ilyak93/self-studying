#include <string>
#include <functional>

class SessionKey {
private:
    int sessionId;
    int leaderId;

public:
    SessionKey(int sessionId, int leaderId) : sessionId(sessionId), leaderId(leaderId) {}

    bool operator==(const SessionKey& other) const {
        return (this->sessionId == other.sessionId) && (this->leaderId == other.leaderId);
    }

    std::string toString() const {
        return std::to_string(this->sessionId) + "," + std::to_string(this->leaderId);
    }

    size_t hash() const {
        return std::hash<int>{}(sessionId) ^ (std::hash<int>{}(leaderId) << 1);
    }
};