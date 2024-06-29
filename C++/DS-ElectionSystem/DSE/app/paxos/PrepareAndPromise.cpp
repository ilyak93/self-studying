// PrepareAndPromise.cpp
#include "PrepareAndPromise.hpp"
#include "VotesMap.hpp"

PrepareAndPromise::PrepareAndPromise(const protos::Prepare& prepare, const protos::Session& session) {
    std::shared_ptr<Vote> currentMapVote = VotesMap::get(prepare.voterid());

    if (prepare.roundnumber() > session.lastround()) {
        if (currentMapVote == nullptr) {
            promise.set_roundnumber(prepare.roundnumber());
            promise.set_ack(true);
            promise.set_lastgoodround(session.lastgoodround());
            promise.set_serverid(session.serverid());
            promise.set_sessionid(session.sessionid());
            promise.set_leaderid(session.leaderid());

            this->session.set_sessionid(session.sessionid());
            this->session.set_serverid(session.serverid());
            this->session.set_lastround(prepare.roundnumber());
            this->session.set_lastgoodround(session.lastgoodround());
            this->session.set_isdecided(session.isdecided());
            this->session.set_leaderid(session.leaderid());
        } else {
            protos::VotePax* currentMapVotePax = promise.mutable_vote();
            currentMapVotePax->set_clientid(currentMapVote->getClientId());
            currentMapVotePax->set_party(currentMapVote->getParty());
            currentMapVotePax->set_originstate(currentMapVote->getOriginState());
            currentMapVotePax->set_currentstate(currentMapVote->getCurrentState());
            currentMapVotePax->set_timestamp(currentMapVote->getTimeStamp());
            currentMapVotePax->set_sessionid(session.sessionid());
            currentMapVotePax->set_leaderid(session.leaderid());

            promise.set_roundnumber(prepare.roundnumber());
            promise.set_ack(true);
            promise.set_lastgoodround(session.lastgoodround());
            promise.set_serverid(session.serverid());
            promise.set_sessionid(session.sessionid());
            promise.set_leaderid(session.leaderid());

            this->session.set_sessionid(session.sessionid());
            this->session.set_serverid(session.serverid());
            this->session.set_lastround(prepare.roundnumber());
            this->session.set_lastgoodround(session.lastgoodround());
            this->session.mutable_vote()->CopyFrom(*currentMapVotePax);
            this->session.set_isdecided(session.isdecided());
            this->session.set_leaderid(session.leaderid());
        }
    } else {
        promise.set_roundnumber(prepare.roundnumber());
        promise.set_ack(false);
        promise.set_lastgoodround(session.lastgoodround());
        if (session.has_vote()) {
            promise.mutable_vote()->CopyFrom(session.vote());
        }
        promise.set_serverid(session.serverid());
        promise.set_sessionid(session.sessionid());
        promise.set_leaderid(session.leaderid());

        if (currentMapVote == nullptr) {
            this->session.set_sessionid(session.sessionid());
            this->session.set_serverid(session.serverid());
            this->session.set_lastround(session.lastround());
            this->session.set_lastgoodround(session.lastgoodround());
            this->session.set_isdecided(session.isdecided());
            this->session.set_leaderid(session.leaderid());
        } else {
            protos::VotePax* currentMapVotePax = this->session.mutable_vote();
            currentMapVotePax->set_clientid(currentMapVote->getClientId());
            currentMapVotePax->set_party(currentMapVote->getParty());
            currentMapVotePax->set_originstate(currentMapVote->getOriginState());
            currentMapVotePax->set_currentstate(currentMapVote->getCurrentState());
            currentMapVotePax->set_timestamp(currentMapVote->getTimeStamp());
            currentMapVotePax->set_sessionid(session.sessionid());
            currentMapVotePax->set_leaderid(session.leaderid());

            this->session.set_sessionid(session.sessionid());
            this->session.set_serverid(session.serverid());
            this->session.set_lastround(session.lastround());
            this->session.set_lastgoodround(session.lastgoodround());
            this->session.set_isdecided(session.isdecided());
            this->session.set_leaderid(session.leaderid());
        }
    }
}

protos::Promise PrepareAndPromise::getPromise() const {
    return promise;
}