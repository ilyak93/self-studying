#include "SessionsMap.hpp"

// Implementation for SessionKey
SessionKey::SessionKey(int sessionId, int leaderId)
    : sessionId(sessionId), leaderId(leaderId) {}

bool SessionKey::operator==(const SessionKey& other) const {
    return sessionId == other.sessionId && leaderId == other.leaderId;
}

std::size_t SessionKey::Hash::operator()(const SessionKey& key) const {
    return std::hash<int>()(key.sessionId) ^ std::hash<int>()(key.leaderId);
}

// Static member definitions
std::unordered_map<SessionKey, protos::Session, SessionKey::Hash> SessionsMap::sessionsMap;
std::mutex SessionsMap::mapMutex;
std::atomic<int> SessionsMap::sessionsCounter{0};

// Implementation for SessionsMap
protos::Session SessionsMap::createNewSession(int leaderId, int serverId, int voterID) {
    int newSessionId = sessionsCounter.fetch_add(1);
    protos::Session newSession = createSession(newSessionId, leaderId, serverId, voterID);
    return newSession;
}

protos::Session SessionsMap::createNewSession(int leaderId, int serverId, int voterID, int sessionId) {
    protos::Session newSession = createSession(sessionId, leaderId, serverId, voterID);
    SessionKey sessionKey(sessionId, leaderId);
    put(sessionKey, newSession);
    return newSession;
}

std::shared_ptr<protos::Session> SessionsMap::get(const SessionKey& sessionKey) {
    std::lock_guard<std::mutex> lock(mapMutex);
    auto it = sessionsMap.find(sessionKey);
    if (it != sessionsMap.end()) {
        return std::make_shared<protos::Session>(it->second);
    }
    return nullptr;
}

void SessionsMap::put(const SessionKey& sessionKey, const protos::Session& session) {
    std::lock_guard<std::mutex> lock(mapMutex);
    sessionsMap[sessionKey] = session;
}

void SessionsMap::remove(const SessionKey& sessionKey) {
    std::lock_guard<std::mutex> lock(mapMutex);
    sessionsMap.erase(sessionKey);
}

protos::Session SessionsMap::createSession(int sessionId, int leaderId, int serverId, int voterID) {
    protos::Session newSession;
    newSession.set_sessionid(sessionId);
    newSession.set_serverid(serverId);
    newSession.set_lastround(0);
    newSession.set_lastgoodround(0);
    newSession.set_isdecided(false);
    newSession.set_leaderid(leaderId);
    newSession.set_voterid(voterID);
    return newSession;
}
