// PaxosAction.h
#ifndef PAXOS_ACTION_H
#define PAXOS_ACTION_H

#include "Paxos.pb.h"  // This includes the generated protobuf header

namespace app {
namespace paxos {

class PaxosAction {
protected:
    protos::Session session;  // Use the protos namespace

public:
    PaxosAction() = default;
    explicit PaxosAction(const protos::Session& sess) : session(sess) {}

    const protos::Session& getSession() const {
        return this->session;
    }

    protos::Session* mutable_session() {
        return &this->session;
    }
};

} // namespace paxos
} // namespace app

#endif // PAXOS_ACTION_H