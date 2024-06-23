#ifndef VOTECONTROLLER_HPP
#define VOTECONTROLLER_HPP

#include <crow.h>
#include <vector>
#include <string>
#include <CommandElections.hpp>
#include "json/json.h"


class VoteController {
public:
    VoteController();

    crow::response all();
    crow::response one(int clientId);
    crow::response countVotesInState();
    crow::response countAllVotes();
    crow::response getSingleStatePartyStatus(const VotesCountKey& key);
    crow::response getDistribution();
    crow::response getWinner();
    crow::response newVote(const Vote& newVote);
    crow::response startElections(const CommandElections& commandElections);

private:
    std::string jsonify(const std::vector<std::shared_ptr<Vote>>& votes);
    std::string jsonify(const std::unordered_map<std::string, std::shared_ptr<VotesCount>>& votesCounts);
    std::string jsonify(const std::vector<Distribution>& distributions);
};

#endif // VOTECONTROLLER_HPP