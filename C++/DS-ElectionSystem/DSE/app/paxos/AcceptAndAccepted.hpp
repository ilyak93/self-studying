#ifndef ACCEPT_AND_ACCEPTED_HPP
#define ACCEPT_AND_ACCEPTED_HPP

#include "paxos.pb.h"
#include "PaxosAction.hpp"

class AcceptAndAccepted : public app::paxos::PaxosAction {
private:
    protos::Accepted accepted;

public:
    AcceptAndAccepted(const protos::Accept& accept, const protos::Session& session);
    const protos::Accepted& getAccepted() const;
};

#endif // ACCEPT_AND_ACCEPTED_HPP