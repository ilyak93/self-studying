#include "VoteController.hpp"
#include "Server.hpp"
#include <ControllerMessage.hpp>


VoteController::VoteController() {}

crow::response VoteController::all() {
    auto votes = VotesMap::getAll();
    return crow::response(jsonify(votes));
}

crow::response VoteController::one(int clientId) {
    ControllerMessage controllerMessage;
    auto vote = VotesMap::get(clientId);
    if (!vote) {
        controllerMessage.setMessage("The client " + std::to_string(clientId) + " is not saved in this server. Try another server or try later.");
        return crow::response(404, controllerMessage.getMessage());
    }
    controllerMessage.setMessage(vote->toString());
    return crow::response(controllerMessage.getMessage());
}

crow::response VoteController::countVotesInState() {
    auto votesCounts = VotesMap::countVotes();
    return crow::response(jsonify(votesCounts));
}

crow::response VoteController::countAllVotes() {
    std::vector<VotesCount> allVotesCounts;
    try {
        auto grpcEndElections = GreetingClient(Server::zkManager.getAddressInEachState());
        allVotesCounts = grpcEndElections.getAllVotesCounts().values();
    } catch (const std::exception& e) {
        std::cerr << "Exception in countAllVotes: " << e.what() << std::endl;
    }
    return crow::response(jsonify(allVotesCounts));
}

crow::response VoteController::getSingleStatePartyStatus(const VotesCountKey& key) {
    VotesCount status;
    try {
        auto grpcStatus = GreetingClient(Server::zkManager.getAddressInAnotherState(key.getState()));
        status = grpcStatus.getStatus(key.getParty(), key.getState());
    } catch (const std::exception& e) {
        std::cerr << "Exception in getSingleStatePartyStatus: " << e.what() << std::endl;
    }
    Json::Value json;
    json["party"] = status.getParty();
    json["count"] = status.getCount();
    json["state"] = status.getState();
    return crow::response(json.toStyledString());
}

crow::response VoteController::getDistribution() {
    std::map<VotesCountKey, VotesCount> allVotesCountsMap;
    try {
        auto grpcDistribution = GreetingClient(Server::zkManager.getAddressInEachState());
        allVotesCountsMap = grpcDistribution.getAllVotesCounts();
    } catch (const std::exception& e) {
        std::cerr << "Exception in getDistribution: " << e.what() << std::endl;
    }
    auto distributions = VotesMap::getDistribution(allVotesCountsMap, Server::stateToElectors);
    return crow::response(jsonify(distributions));
}

crow::response VoteController::getWinner() {
    ControllerMessage controllerMessage("The winner is still unknown");
    if (Server::choseWinner) {
        std::string winner = Server::getWinner();
        controllerMessage.setMessage("The winner is: " + winner);
    }
    return crow::response(controllerMessage.getMessage());
}

crow::response VoteController::newVote(const Vote& newVote) {
    ControllerMessage controllerMessage;

    if (Server::electionsEnded) {
        controllerMessage.setMessage("Elections ended. Cannot receive new votes.");
        return crow::response(400, controllerMessage.getMessage());
    }
    if (!newVote.isValid()) {
        controllerMessage.setMessage("The client ID does not appear in the voters registration file.");
        return crow::response(400, controllerMessage.getMessage());
    }

    if (!Server::electionsStarted || !Server::receiveNewVotes || Server::electionsEnded) {
        controllerMessage.setMessage("Out of elections time. Cannot receive new votes.");
        return crow::response(400, controllerMessage.getMessage());
    }
    if (newVote.getOriginState() != newVote.getCurrentState()) {
        Future<Vote> future = FutureRemoteGreetingClient().calculate(newVote);
        int voteNumber = Server::votesCounter.addAndGet(1);
        Server::votesInDistributionProcess[voteNumber] = future;
    } else {
        try {
            Future<Vote> future = FuturePaxosGreetingClient().calculate(Server::zkManager.getCurrentStateAddressesForPaxos(), Server::serverId, newVote);
            int voteNumber = Server::votesCounter.addAndGet(1);
            Server::votesInDistributionProcess[voteNumber] = future;
        } catch (const std::exception& e) {
            std::cerr << "Exception in newVote: " << e.what() << std::endl;
        }
    }
    controllerMessage.setMessage("The Elections system received the vote Successfully\n" 
                                 + "clientID: " + std::to_string(newVote.getClientId()) + "\n" 
                                 + "party: " + newVote.getParty() + "\n"
                                 + "current state: " + newVote.getCurrentState() + "\n"
                                 + "origin state: " + newVote.getOriginState() + "\n"
                                 + "timestamp: " + std::to_string(newVote.getTimeStamp()) + "\n");
    return crow::response(controllerMessage.getMessage());
}

crow::response VoteController::startElections(const CommandElections& commandElections) {
    ControllerMessage controllerMessage("Unknown controller command");
    std::string command = commandElections.getCommand();
    if (command == "start") {
        if (Server::electionsStarted) {
            controllerMessage.setMessage("Elections already started");
        } else if (!Server::electionsStarted && Server::electionsEnded) {
            controllerMessage.setMessage("Elections ended before they started");
        } else {
            auto grpcStartElections = GreetingClient(Server::zkManager.getAddressesToStartElections());
            grpcStartElections.startElections();
            controllerMessage.setMessage("Started elections");
        }
    }
    if (command == "end") {
        if (Server::receiveNewVotes) {
            Server::receiveNewVotes = false;
            auto grpcEndElections = GreetingClient(Server::zkManager.getAddressesToStartElections());
            grpcEndElections.endElections();
            controllerMessage.setMessage("Ended elections");
        }
    }
    if (command == "isfinished") {
        if (Server::finishedAll) {
            controllerMessage.setMessage("Finished all counting");
        } else {
            controllerMessage.setMessage("Still in process");
        }
    }
    if (command == "isStarted") {
        if (Server::electionsStarted) {
            controllerMessage.setMessage("Elections started");
        } else {
            controllerMessage.setMessage("Elections not started");
        }
    }
    if (command == "isEnded") {
        if (Server::electionsEnded) {
            controllerMessage.setMessage("Elections ended");
        } else {
            controllerMessage.setMessage("Elections not ended");
        }
    }
    return crow::response(controllerMessage.getMessage());
}

std::string VoteController::jsonify(const std::vector<std::shared_ptr<Vote>>& votes) {
    Json::Value jsonVotes;
    for (const auto& votePtr : votes) {
        Json::Value jsonVote;
        jsonVote["clientId"] = votePtr->getClientId();
        jsonVote["party"] = votePtr->getParty();
        jsonVote["originState"] = votePtr->getOriginState();
        jsonVote["currentState"] = votePtr->getCurrentState();
        jsonVote["timeStamp"] = votePtr->getTimeStamp();
        jsonVotes.append(jsonVote);
    }
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, jsonVotes);
}

std::string VoteController::jsonify(const std::unordered_map<std::string, std::shared_ptr<VotesCount>>& votesCounts) {
    Json::Value jsonVotesCounts;
    for (const auto& pair : votesCounts) {
        const auto& votesCountPtr = pair.second;
        Json::Value jsonVotesCount;
        jsonVotesCount["party"] = votesCountPtr->getParty();
        jsonVotesCount["count"] = votesCountPtr->getCount();
        jsonVotesCount["state"] = votesCountPtr->getState();
        jsonVotesCounts.append(jsonVotesCount);
    }
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, jsonVotesCounts);
}

std::string VoteController::jsonify(const std::vector<Distribution>& distributions) {
    Json::Value jsonDistributions;
    for (const auto& distribution : distributions) {
        Json::Value jsonDistribution;
        jsonDistribution["party"] = distribution.getParty();
        jsonDistribution["electors"] = distribution.getElectors();
        jsonDistributions.append(jsonDistribution);
    }
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, jsonDistributions);
}