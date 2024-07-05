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

    // Getter methods for hashing
    int getSessionId() const { return sessionId; }
    int getLeaderId() const { return leaderId; }

    // Nested Hash struct
    struct Hash {
        std::size_t operator()(const SessionKey& key) const {
            return std::hash<int>{}(key.getSessionId()) ^ (std::hash<int>{}(key.getLeaderId()) << 1);
        }
    };
};