#include "VotesMap.hpp"
#include <iostream>
#include <algorithm>

std::unordered_map<int, std::shared_ptr<Vote>> VotesMap::votesMap;
std::mutex VotesMap::mutex;
std::unordered_map<std::string, std::shared_ptr<VotesCount>> VotesMap::countedVotes;

std::shared_ptr<Vote> VotesMap::get(int clientId) {
    std::lock_guard<std::mutex> lock(mutex);
    return votesMap[clientId];
}

void VotesMap::put(int clientId, std::shared_ptr<Vote> vote) {
    std::lock_guard<std::mutex> lock(mutex);
    votesMap[clientId] = vote;
}

std::vector<std::shared_ptr<Vote>> VotesMap::getAll() {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<std::shared_ptr<Vote>> votes;
    for (const auto& pair : votesMap) {
        votes.push_back(pair.second);
    }
    return votes;
}

std::vector<std::shared_ptr<Vote>> VotesMap::copyAll() {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<std::shared_ptr<Vote>> broadcastVotes;
    for (const auto& pair : votesMap) {
        broadcastVotes.push_back(pair.second);
    }
    return broadcastVotes;
}

int VotesMap::size() {
    std::lock_guard<std::mutex> lock(mutex);
    return votesMap.size();
}

std::unordered_map<std::string, std::shared_ptr<VotesCount>> VotesMap::countVotes() {
    std::vector<std::shared_ptr<Vote>> votes;
    {
        std::lock_guard<std::mutex> lock(mutex);
        votes = getAll();
    }

    std::unordered_map<std::string, std::shared_ptr<VotesCount>> votesCounts;
    for (const auto& vote : votes) {
        auto it = votesCounts.find(vote->getParty());
        if (it == votesCounts.end()) {
            votesCounts[vote->getParty()] = std::make_shared<VotesCount>(vote->getParty());
        } else {
            it->second->add();
        }
    }

    {
        std::lock_guard<std::mutex> lock(mutex);
        countedVotes = votesCounts;
    }

    return votesCounts;
}

std::vector<Distribution> VotesMap::getDistribution(const std::unordered_map<VotesCountKey, std::shared_ptr<VotesCount>>& allVotesCounts, const std::unordered_map<std::string, int>& stateToElectors) {
    std::unordered_map<std::string, std::unordered_map<std::string, int>> stateKeyToPartyAndVotesMap;

    for (const auto& pair : allVotesCounts) {
        const auto& votesCount = pair.second;
        stateKeyToPartyAndVotesMap[votesCount->getState()][votesCount->getParty()] = votesCount->getCount();
    }

    std::unordered_map<std::string, std::shared_ptr<VotesCount>> votesCountMap;
    for (const auto& stateEntry : stateKeyToPartyAndVotesMap) {
        const auto& state = stateEntry.first;
        const auto& partyAndVotesMap = stateEntry.second;

        std::string winningParty;
        int winningVotes = 0;
        for (const auto& partyVotes : partyAndVotesMap) {
            if (partyVotes.second > winningVotes) {
                winningParty = partyVotes.first;
                winningVotes = partyVotes.second;
            }
        }

        votesCountMap[state] = std::make_shared<VotesCount>(winningParty, stateToElectors.at(state), state);
    }

    std::unordered_map<std::string, int> partyToElectors;
    for (const auto& pair : votesCountMap) {
        const auto& votesCount = pair.second;
        partyToElectors[votesCount->getParty()] += votesCount->getCount();
    }

    std::vector<Distribution> distributions;
    for (const auto& pair : partyToElectors) {
        distributions.emplace_back(pair.first, pair.second);
    }

    return distributions;
}