#ifndef VOTESMAP_HPP
#define VOTESMAP_HPP

#include <unordered_map>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "Vote.hpp"
#include "Distribution.hpp"
#include "VotesCountKey.hpp"
#include "VotesCount.hpp"


class VotesMap {
private:
    

public:
    static std::unordered_map<int, std::shared_ptr<Vote>> votesMap;
    static std::mutex mutex;
    static std::unordered_map<std::string, std::shared_ptr<VotesCount>> countedVotes;

    static std::shared_ptr<Vote> get(int clientId);
    static void put(int clientId, std::shared_ptr<Vote> vote);
    static std::vector<std::shared_ptr<Vote>> getAll();
    static std::vector<std::shared_ptr<Vote>> copyAll();
    static int size();
    static std::unordered_map<std::string, std::shared_ptr<VotesCount>> countVotes();
    static std::vector<Distribution> getDistribution(const std::unordered_map<VotesCountKey, std::shared_ptr<VotesCount>>& allVotesCounts, const std::unordered_map<std::string, int>& stateToElectors);
};

#endif // VOTESMAP_HPP