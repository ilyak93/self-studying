#include "AcceptAndAccepted.hpp"
#include "VotesMap.hpp"
#include "Vote.hpp"
#include <iostream>

AcceptAndAccepted::AcceptAndAccepted(const protos::Accept& accept, const protos::Session& session)
    : PaxosAction() {
    
    std::cout << "inside AcceptAndAccepted" << std::endl;
    std::cout << "accept vote is: " << accept.vote().ShortDebugString() << std::endl;
    std::cout << "session vote is: " << session.vote().ShortDebugString() << std::endl;

    std::shared_ptr<Vote> currentMapVote = VotesMap::get(accept.vote().clientid());
    if (currentMapVote == nullptr) {
        std::cout << "currentMapVote == nullptr" << std::endl;
        std::cout << "accept.getRoundNumber() = " << accept.roundnumber() << std::endl;
        std::cout << "session.getLastRound() = " << session.lastround() << std::endl;
        if ((accept.roundnumber() == session.lastround()) || (session.lastround() == 0)) {
            std::cout << "Ack is true" << std::endl;

            this->session = protos::Session();
            this->session.set_sessionid(session.sessionid());
            this->session.set_serverid(session.serverid());
            this->session.set_lastround(accept.roundnumber());
            this->session.set_lastgoodround(accept.roundnumber());
            *(this->session.mutable_vote()) = accept.vote();
            this->session.set_isdecided(true);
            this->session.set_leaderid(session.leaderid());

            this->accepted = protos::Accepted();
            this->accepted.set_roundnumber(accept.roundnumber());
            this->accepted.set_ack(true);
            *(this->accepted.mutable_vote()) = accept.vote();
            this->accepted.set_serverid(session.serverid());
            this->accepted.set_sessionid(session.sessionid());
            this->accepted.set_leaderid(session.leaderid());
        } else {
            std::cout << "Ack is false" << std::endl;

            this->session = protos::Session();
            this->session.set_sessionid(session.sessionid());
            this->session.set_serverid(session.serverid());
            this->session.set_lastround(session.lastround());
            this->session.set_lastgoodround(session.lastgoodround());
            this->session.set_isdecided(session.isdecided());
            this->session.set_leaderid(session.leaderid());

            this->accepted = protos::Accepted();
            this->accepted.set_roundnumber(accept.roundnumber());
            this->accepted.set_ack(false);
            this->accepted.set_serverid(session.serverid());
            this->accepted.set_sessionid(session.sessionid());
            this->accepted.set_leaderid(session.leaderid());
        }
    } else {
        std::cout << "currentMapVote != nullptr" << std::endl;
        std::cout << "accept.getVote().getTimeStamp() = " << accept.vote().timestamp() << std::endl;
        std::cout << "currentMapVote.getTimeStamp() = " << currentMapVote->getTimeStamp() << std::endl;
        std::cout << "accept.getRoundNumber() = " << accept.roundnumber() << std::endl;
        std::cout << "session.getLastRound() = " << session.lastround() << std::endl;
        if ((accept.vote().timestamp() >= currentMapVote->getTimeStamp()) && 
            (accept.roundnumber() == session.lastround()) || (session.lastround() == 0)) {
            std::cout << "Ack is true" << std::endl;

            this->session = protos::Session();
            this->session.set_sessionid(session.sessionid());
            this->session.set_serverid(session.serverid());
            this->session.set_lastround(accept.roundnumber());
            this->session.set_lastgoodround(accept.roundnumber());
            *(this->session.mutable_vote()) = accept.vote();
            this->session.set_isdecided(true);
            this->session.set_leaderid(session.leaderid());

            this->accepted = protos::Accepted();
            this->accepted.set_roundnumber(accept.roundnumber());
            this->accepted.set_ack(true);
            *(this->accepted.mutable_vote()) = accept.vote();
            this->accepted.set_serverid(session.serverid());
            this->accepted.set_sessionid(session.sessionid());
            this->accepted.set_leaderid(session.leaderid());
        } else {
            std::cout << "Ack is false" << std::endl;

            protos::VotePax currentMapVotePax;
            currentMapVotePax.set_clientid(currentMapVote->getClientId());
            currentMapVotePax.set_party(currentMapVote->getParty());
            currentMapVotePax.set_originstate(currentMapVote->getOriginState());
            currentMapVotePax.set_currentstate(currentMapVote->getCurrentState());
            currentMapVotePax.set_timestamp(currentMapVote->getTimeStamp());
            currentMapVotePax.set_sessionid(session.sessionid());
            currentMapVotePax.set_leaderid(session.leaderid());

            this->session = protos::Session();
            this->session.set_sessionid(session.sessionid());
            this->session.set_serverid(session.serverid());
            this->session.set_lastround(session.lastround());
            this->session.set_lastgoodround(session.lastgoodround());
            *(this->session.mutable_vote()) = currentMapVotePax;
            this->session.set_isdecided(session.isdecided());
            this->session.set_leaderid(session.leaderid());

            this->accepted = protos::Accepted();
            this->accepted.set_roundnumber(accept.roundnumber());
            this->accepted.set_ack(false);
            *(this->accepted.mutable_vote()) = currentMapVotePax;
            this->accepted.set_serverid(session.serverid());
            this->accepted.set_sessionid(session.sessionid());
            this->accepted.set_leaderid(session.leaderid());
        }
    }
}

const protos::Accepted& AcceptAndAccepted::getAccepted() const {
    return this->accepted;
}