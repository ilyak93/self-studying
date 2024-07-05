#include "GreetingPaxosClient.hpp"
#include <chrono>
#include <algorithm>
#include <iostream>

GreetingPaxosClient::GreetingPaxosClient(const std::vector<std::string>& addresses, int serverId)
    : serverId(serverId) {
    for (const auto& address : addresses) {
        auto channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
        channels.push_back(channel);
        stubs.push_back(protos::PaxosGreeter::NewStub(channel));
    }
}

void GreetingPaxosClient::shutdown() {
    for (auto& channel : channels) {
        channel->GetState(true);
    }
}

OneTimeUseElement<std::future<protos::Init>> GreetingPaxosClient::sendInit(const protos::Init& init, protos::PaxosGreeter::Stub* stub) {
    grpc::ClientContext context;
    std::promise<protos::Init> promise;
    auto future = promise.get_future();
    stub->async()->UponReceivingInit(&context, &init, &promise.get_future().get(), [&promise](grpc::Status status) {
        if (status.ok()) {
            promise.set_value(std::move(promise.get_future().get()));
        } else {
            promise.set_exception(std::make_exception_ptr(std::runtime_error(status.error_message())));
        }
    });
    return OneTimeUseElement<std::future<protos::Init>>(std::move(future));
}

OneTimeUseElement<std::future<protos::Promise>> GreetingPaxosClient::sendPrepare(const protos::Prepare& prepare, protos::PaxosGreeter::Stub* stub) {
    grpc::ClientContext context;
    std::promise<protos::Promise> promise;
    auto future = promise.get_future();
    stub->async()->UponReceivingPrepare(&context, &prepare, &promise.get_future().get(), [&promise](grpc::Status status) {
        if (status.ok()) {
            promise.set_value(std::move(promise.get_future().get()));
        } else {
            promise.set_exception(std::make_exception_ptr(std::runtime_error(status.error_message())));
        }
    });
    return OneTimeUseElement<std::future<protos::Promise>>(std::move(future));
}

OneTimeUseElement<std::future<protos::Accepted>> GreetingPaxosClient::sendAccept(const protos::Accept& accept, protos::PaxosGreeter::Stub* stub) {
    grpc::ClientContext context;
    std::promise<protos::Accepted> promise;
    auto future = promise.get_future();
    stub->async()->UponReceivingAccept(&context, &accept, &promise.get_future().get(), [&promise](grpc::Status status) {
        if (status.ok()) {
            promise.set_value(std::move(promise.get_future().get()));
        } else {
            promise.set_exception(std::make_exception_ptr(std::runtime_error(status.error_message())));
        }
    });
    return OneTimeUseElement<std::future<protos::Accepted>>(std::move(future));
}

OneTimeUseElement<std::future<protos::Finish>> GreetingPaxosClient::sendCommit(const protos::Commit& commit, protos::PaxosGreeter::Stub* stub) {
    grpc::ClientContext context;
    std::promise<protos::Finish> promise;
    auto future = promise.get_future();
    stub->async()->UponReceivingCommit(&context, &commit, &promise.get_future().get(), [&promise](grpc::Status status) {
        if (status.ok()) {
            promise.set_value(std::move(promise.get_future().get()));
        } else {
            promise.set_exception(std::make_exception_ptr(std::runtime_error(status.error_message())));
        }
    });
    return OneTimeUseElement<std::future<protos::Finish>>(std::move(future));
}

Vote GreetingPaxosClient::sendVote(const Vote& vote) {
    int roundNumber = 0;
    bool isDecided = false;
    Vote decidedVote = vote;

    auto session = SessionsMap::createNewSession(this->serverId, this->serverId, vote.getClientId());

    int sessionID = session.sessionid();
    int leaderID = session.leaderid();

    protos::Init init;
    init.set_serverid(this->serverId);
    init.set_sessionid(sessionID);
    init.set_leaderid(leaderID);
    init.set_voterid(vote.getClientId());

    std::vector<OneTimeUseElement<std::future<protos::Init>>> initFutures;
    for (auto& stub : stubs) {
        initFutures.push_back(sendInit(init, stub.get()));
    }

    int receivedCounter = 0;
    while(receivedCounter < stubs.size()) {
        for (auto& initFuture : initFutures) {
            if (!initFuture.isUsed() && initFuture.getElement().wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                try {
                    auto initReply = initFuture.getElement().get();
                    receivedCounter++;
                } catch (const std::exception& e) {
                    std::cout << "Exception in GreetingPaxosClient::sendVote while trying to get value of init future: " << e.what() << std::endl;
                }
            }
        }
    }

    while (!isDecided) {
        roundNumber++;
        protos::Prepare prepare;
        prepare.set_roundnumber(roundNumber);
        prepare.set_timestamp(vote.getTimeStamp());
        prepare.set_serverid(this->serverId);
        prepare.set_sessionid(sessionID);
        prepare.set_leaderid(leaderID);
        prepare.set_voterid(vote.getClientId());

        std::vector<OneTimeUseElement<std::future<protos::Promise>>> promiseFutures;
        std::vector<protos::Promise> promises;
        for (auto& stub : stubs) {
            promiseFutures.push_back(sendPrepare(prepare, stub.get()));
        }

        receivedCounter = 0;
        while (receivedCounter < stubs.size() / 2) {
            for (auto& promiseFuture : promiseFutures) {
                if (!promiseFuture.isUsed() && promiseFuture.getElement().wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    try {
                        auto promise = promiseFuture.getElement().get();
                        receivedCounter++;
                        promises.push_back(promise);
                    } catch (const std::exception& e) {
                        std::cout << "Exception in GreetingPaxosClient::sendVote while trying to get value of promise future: " << e.what() << std::endl;
                    }
                }
            }
        }

        int acksInCurrentRoundCounter = 0;
        for (const auto& promise : promises) {
            if (promise.ack() && promise.roundnumber() == roundNumber) {
                acksInCurrentRoundCounter++;
            }
        }

        std::cout << "In Paxos Leader sessionId = " << sessionID << " leaderId = " << leaderID 
                  << " round = " << roundNumber << " from promises acksInCurrentRoundCounter = " << acksInCurrentRoundCounter << std::endl;

        if (acksInCurrentRoundCounter > stubs.size() / 2) {
            protos::VotePax voteWithMaxTimeStamp;
            voteWithMaxTimeStamp.set_clientid(vote.getClientId());
            voteWithMaxTimeStamp.set_party(vote.getParty());
            voteWithMaxTimeStamp.set_originstate(vote.getOriginState());
            voteWithMaxTimeStamp.set_currentstate(vote.getCurrentState());
            voteWithMaxTimeStamp.set_timestamp(vote.getTimeStamp());
            voteWithMaxTimeStamp.set_sessionid(sessionID);
            voteWithMaxTimeStamp.set_leaderid(leaderID);

            for (const auto& promise : promises) {
                if (promise.vote().timestamp() > voteWithMaxTimeStamp.timestamp()) {
                    voteWithMaxTimeStamp = promise.vote();
                }
            }

            protos::Accept accept;
            accept.set_roundnumber(roundNumber);
            *accept.mutable_vote() = voteWithMaxTimeStamp;
            accept.set_serverid(this->serverId);
            accept.set_sessionid(sessionID);
            accept.set_leaderid(leaderID);

            std::vector<OneTimeUseElement<std::future<protos::Accepted>>> acceptedFutures;
            std::vector<protos::Accepted> accepteds;
            for (auto& stub : stubs) {
                acceptedFutures.push_back(sendAccept(accept, stub.get()));
            }

            receivedCounter = 0;
            while (receivedCounter < stubs.size() / 2) {
                for (auto& acceptedFuture : acceptedFutures) {
                    if (!acceptedFuture.isUsed() && acceptedFuture.getElement().wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                        try {
                            auto accepted = acceptedFuture.getElement().get();
                            receivedCounter++;
                            accepteds.push_back(accepted);
                        } catch (const std::exception& e) {
                            std::cout << "Exception in GreetingPaxosClient::sendVote while trying to get value of accepted future: " << e.what() << std::endl;
                        }
                    }
                }
            }

            acksInCurrentRoundCounter = 0;
            for (const auto& accepted : accepteds) {
                if (accepted.ack() && accepted.roundnumber() == roundNumber) {
                    acksInCurrentRoundCounter++;
                }
            }

            std::cout << "In Paxos Leader sessionId = " << sessionID << " leaderId = " << leaderID 
                      << " round = " << roundNumber << " from accepted acksInCurrentRoundCounter = " << acksInCurrentRoundCounter << std::endl;

            if (acksInCurrentRoundCounter > stubs.size() / 2) {
                decidedVote = Vote(voteWithMaxTimeStamp.clientid(),
                                   voteWithMaxTimeStamp.party(),
                                   voteWithMaxTimeStamp.originstate(),
                                   voteWithMaxTimeStamp.currentstate(),
                                   voteWithMaxTimeStamp.timestamp());

                protos::Commit commit;
                commit.set_sessionid(sessionID);
                commit.set_leaderid(leaderID);
                *commit.mutable_vote() = voteWithMaxTimeStamp;

                std::vector<OneTimeUseElement<std::future<protos::Finish>>> finishFutures;
                for (auto& stub : stubs) {
                    finishFutures.push_back(sendCommit(commit, stub.get()));
                }

                receivedCounter = 0;
                while (receivedCounter < stubs.size()) {
                    for (auto& finishFuture : finishFutures) {
                        if (!finishFuture.isUsed() && finishFuture.getElement().wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                            try {
                                finishFuture.getElement().get();
                                receivedCounter++;
                            } catch (const std::exception& e) {
                                std::cout << "Exception in GreetingPaxosClient::sendVote while trying to get value of finish future: " << e.what() << std::endl;
                            }
                        }
                    }
                }
                isDecided = true;
            }
        }
    }
    return decidedVote;
}