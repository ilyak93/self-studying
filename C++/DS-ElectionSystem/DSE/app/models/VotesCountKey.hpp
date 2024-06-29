#ifndef VOTESCOUNTKEY_HPP
#define VOTESCOUNTKEY_HPP

#include <string>
#include <functional>

class VotesCountKey {
private:
    std::string party;
    std::string state;

public:
    // Constructor
    VotesCountKey(const std::string& party, const std::string& state);

    // Setter methods
    VotesCountKey& setParty(const std::string& party);
    VotesCountKey& setState(const std::string& state);

    // Getter methods
    std::string getParty() const;
    std::string getState() const;

    // Hash function
    size_t hash() const;

    // Equality operator
    bool operator==(const VotesCountKey& other) const;
};

// Specialization of std::hash for VotesCountKey
namespace std {
    template <>
    struct hash<VotesCountKey> {
        size_t operator()(const VotesCountKey& key) const {
            return key.hash();
        }
    };
}

#endif // VOTESCOUNTKEY_HPP