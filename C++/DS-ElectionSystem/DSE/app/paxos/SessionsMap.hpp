#ifndef SESSIONSMAP_HPP
#define SESSIONSMAP_HPP

#include <atomic>
#include <unordered_map>
#include <mutex>
#include <memory>
#include "Paxos.pb.h"  // Assuming this includes the definition for protos::Paxos::Session and protos::Paxos::VotePax
#include "SessionKey.hpp"

class SessionsMap {
public:
    static protos::Session createNewSession(int leaderId, int serverId, int voterID);
    static protos::Session createNewSession(int leaderId, int serverId, int voterID, int sessionId);
    static std::shared_ptr<protos::Session> get(const SessionKey& sessionKey);
    static void put(const SessionKey& sessionKey, const protos::Session& session);
    static void remove(const SessionKey& sessionKey);

private:
    static protos::Session createSession(int sessionId, int leaderId, int serverId, int voterID);

    static std::unordered_map<SessionKey, protos::Session, SessionKey::Hash> sessionsMap;
    static std::mutex mapMutex;
    static std::atomic<int> sessionsCounter;
};

#endif // SESSIONSMAP_HPP
