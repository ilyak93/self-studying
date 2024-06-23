// SharedRestrictions.hpp
#pragma once
#include <string>
#include <unordered_map>
#include <mutex>

class SharedRestrictions {
public:
    static SharedRestrictions& getInstance() {
        static SharedRestrictions instance;
        return instance;
    }

    void addRestrict(const std::string& client, const std::string& procedure) {
        std::lock_guard<std::mutex> lock(mutex_);
        restrictions_[client] = procedure;
    }

    bool isRestricted(const std::string& client, const std::string& procedure) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = restrictions_.find(client);
        return (it != restrictions_.end() && it->second == procedure);
    }

private:
    SharedRestrictions() = default;
    mutable std::mutex mutex_;
    std::unordered_map<std::string, std::string> restrictions_;

public:
    SharedRestrictions(const SharedRestrictions&) = delete;
    SharedRestrictions& operator=(const SharedRestrictions&) = delete;
};