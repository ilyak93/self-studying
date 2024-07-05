// GreetingClient.cpp

#include "GreetingClient.hpp"
#include "gRPCObjects/interceptors/ClientInterceptor.hpp"
#include <iostream>
#include <chrono>

std::unordered_map<std::string, std::shared_ptr<grpc::Channel>> GreetingClient::remoteChannels;

GreetingClient::GreetingClient(const std::string& address) {
    auto it = remoteChannels.find(address);
    if (it == remoteChannels.end()) {
        singleChannel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
        remoteChannels[address] = singleChannel;
    } else {
        singleChannel = it->second;
    }
}

GreetingClient::GreetingClient(const std::vector<std::string>& addresses) {
    for (const auto& address : addresses) {
        auto it = remoteChannels.find(address);
        if (it == remoteChannels.end()) {
            auto channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
            remoteChannels[address] = channel;
            channels.push_back(channel);
        } else {
            channels.push_back(it->second);
        }
    }
}

GreetingClient::~GreetingClient() {
    shutdown();
}

void GreetingClient::shutdown() {
    for (auto& channel : channels) {
        channel->GetState(true);
    }
    if (singleChannel) {
        singleChannel->GetState(true);
    }
}

std::unique_ptr<protos::Greeter::Stub> GreetingClient::createStub(const std::shared_ptr<grpc::Channel>& channel) {
    return protos::Greeter::NewStub(channel);
}

bool GreetingClient::isAllFutureSendsDone() {
    for (const auto& future : sendsInProcess) {
        if (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            return false;
        }
    }
    return true;
}



std::vector<std::shared_future<protos::VoteReply>> GreetingClient::sendVoteFuture(const Vote& vote) {
    protos::VoteRequest request;
    request.set_clientid(vote.getClientId());
    request.set_party(vote.getParty());
    request.set_originstate(vote.getOriginState());
    request.set_currentstate(vote.getCurrentState());
    request.set_timestamp(vote.getTimeStamp());

    std::vector<std::shared_future<protos::VoteReply>> sendingsInProcess;

    for (auto& channel : channels) {
        auto stub = createStub(channel);
        auto promise = std::make_shared<std::promise<protos::VoteReply>>();
        std::shared_future<protos::VoteReply> shared_future = promise->get_future().share();
        
        auto response = new protos::VoteReply();
        auto context = new grpc::ClientContext();

        stub->async()->ReceiveVote(context, &request, response, 
            [promise, response, context](grpc::Status status) mutable {
                if (status.ok()) {
                    promise->set_value(*response);
                } else {
                    promise->set_exception(std::make_exception_ptr(std::runtime_error(status.error_message())));
                }
                delete response;
                delete context;
            });
        
        sendingsInProcess.push_back(shared_future);
    }
    
    sendsInProcess = sendingsInProcess;
    return sendsInProcess;
}

void GreetingClient::sendVoteBlocking(const Vote& vote) {
    protos::VoteRequest request;
    request.set_clientid(vote.getClientId());
    request.set_party(vote.getParty());
    request.set_originstate(vote.getOriginState());
    request.set_currentstate(vote.getCurrentState());
    request.set_timestamp(vote.getTimeStamp());

    grpc::ClientContext context;
    protos::VoteReply reply;
    auto stub = createStub(singleChannel);
    grpc::Status status = stub->ReceiveVote(&context, request, &reply);
    if (!status.ok()) {
        throw std::runtime_error("RPC failed: " + status.error_message());
    }
}

OneTimeUseElement<std::future<protos::StartElectionsReply>> GreetingClient::sendStartElections(const protos::StartElectionsRequest& request, const std::shared_ptr<grpc::Channel>& channel) {
    auto stub = createStub(channel);
    grpc::ClientContext context;
    std::promise<protos::StartElectionsReply> promise;
    auto future = promise.get_future();
    stub->async()->ReceiveStartElections(&context, &request, &promise.get_future().get(),
        [&promise](grpc::Status status) {
            if (status.ok()) {
                promise.set_value(promise.get_future().get());
            } else {
                promise.set_exception(std::make_exception_ptr(std::runtime_error(status.error_message())));
            }
        });
    return OneTimeUseElement<std::future<protos::StartElectionsReply>>(std::move(future));
}

void GreetingClient::startElections() {
    protos::StartElectionsRequest request;
    std::vector<OneTimeUseElement<std::future<protos::StartElectionsReply>>> startElectionsFutures;
    
    for (auto& channel : channels) {
        startElectionsFutures.push_back(sendStartElections(request, channel));
    }

    int receivedCounter = 0;
    while (receivedCounter < channels.size()) {
        for (auto& startElectionsFuture : startElectionsFutures) {
            if (!startElectionsFuture.isUsed() && startElectionsFuture.getElement().wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                try {
                    startElectionsFuture.getElement().get();
                    startElectionsFuture.use();
                    receivedCounter++;
                } catch (const std::exception& e) {
                    std::cerr << "Caught an exception in GreetingClient::startElections: " << e.what() << std::endl;
                }
            }
        }
    }
}

OneTimeUseElement<std::future<protos::EndElectionsReply>> GreetingClient::sendEndElections(const protos::EndElectionsRequest& request, const std::shared_ptr<grpc::Channel>& channel) {
    auto stub = createStub(channel);
    grpc::ClientContext context;
    std::promise<protos::EndElectionsReply> promise;
    auto future = promise.get_future();
    stub->async()->ReceiveEndElections(&context, &request, &promise.get_future().get(),
        [&promise](grpc::Status status) {
            if (status.ok()) {
                promise.set_value(promise.get_future().get());
            } else {
                promise.set_exception(std::make_exception_ptr(std::runtime_error(status.error_message())));
            }
        });
    return OneTimeUseElement<std::future<protos::EndElectionsReply>>(std::move(future));
}

void GreetingClient::endElections() {
    protos::EndElectionsRequest request;
    std::vector<OneTimeUseElement<std::future<protos::EndElectionsReply>>> endElectionsFutures;
    
    for (auto& channel : channels) {
        endElectionsFutures.push_back(sendEndElections(request, channel));
    }

    int receivedCounter = 0;
    while (receivedCounter < channels.size()) {
        for (auto& endElectionsFuture : endElectionsFutures) {
            if (!endElectionsFuture.isUsed() && endElectionsFuture.getElement().wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                try {
                    endElectionsFuture.getElement().get();
                    endElectionsFuture.use();
                    receivedCounter++;
                } catch (const std::exception& e) {
                    std::cerr << "Caught an exception in GreetingClient::endElections: " << e.what() << std::endl;
                }
            }
        }
    }
}

std::unordered_map<VotesCountKey, std::shared_ptr<VotesCount>> GreetingClient::getAllVotesCounts() {
    std::unordered_map<VotesCountKey, std::shared_ptr<VotesCount>> allVotesCounts;
    protos::VotesCountForPartyRequest request;

    for (auto& channel : channels) {
        auto stub = createStub(channel);
        grpc::ClientContext context;
        std::unique_ptr<grpc::ClientReader<protos::VotesCountForPartyReply>> reader(
            stub->ReciveVotesCount(&context, request));

        protos::VotesCountForPartyReply reply;
        while (reader->Read(&reply)) {
            std::string party = reply.party();
            int count = reply.votescount();
            std::string state = reply.state();
            
            auto votesCount = std::make_shared<VotesCount>(VotesCount(party, count, state));
            VotesCountKey key(party, state);

            auto it = allVotesCounts.find(key);
            if (it == allVotesCounts.end()) {
                allVotesCounts[key] = votesCount;
            } else {
                it->second->add(count);
            }
        }

        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            throw std::runtime_error("RPC failed: " + status.error_message());
        }
    }

    return allVotesCounts;
}

VotesCount GreetingClient::getStatus(const std::string& party, const std::string& state) {
    protos::VotesCountForPartyRequest request;
    request.set_party(party);
    request.set_state(state);

    grpc::ClientContext context;
    protos::VotesCountForPartyReply reply;
    auto stub = createStub(singleChannel);
    grpc::Status status = stub->ReceiveStatus(&context, request, &reply);

    if (!status.ok()) {
        throw std::runtime_error("RPC failed: " + status.error_message());
    }

    return VotesCount(reply.party(), reply.votescount(), reply.state());
}