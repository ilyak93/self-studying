#include "paxos.pb.h" // Assuming this is where the protobuf definitions are
#include "PaxosAction.hpp" // Assuming this is where PaxosAction is defined

class InitAndSession : public app::paxos::PaxosAction {
private:
    protos::Init init;

public:
    InitAndSession(const protos::Init& init, const protos::Session& session)
        : PaxosAction() // Assuming PaxosAction has a default constructor
    {
        this->session = session;
        this->init = protos::Init();
        this->init.set_serverid(init.serverid());
        this->init.set_sessionid(init.sessionid());
        this->init.set_leaderid(session.leaderid());
    }

    const protos::Init& getInit() const {
        return this->init;
    }
};