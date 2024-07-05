// PrepareAndPromise.hpp
#ifndef PREPARE_AND_PROMISE_HPP
#define PREPARE_AND_PROMISE_HPP


#include "Paxos.pb.h"
#include "app/paxos/PaxosAction.hpp"

class PrepareAndPromise : public PaxosAction {
private:
    protos::Promise promise;

public:
    PrepareAndPromise(const protos::Prepare& prepare, const protos::Session& session);
    protos::Promise getPromise() const;
};

#endif // PREPARE_AND_PROMISE_HPP