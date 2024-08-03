#include <iostream>
#include <map>
#include <string>
#include <memory>

class Tenant {
public:
    Tenant(const std::string& name = "") : m_name(name) {}
    std::string name() const { return m_name; }
private:
    std::string m_name;
};

class Registry {
public:
    std::shared_ptr<Tenant> findByName(const std::string& name) {
        auto it = tenants.find(name);
        if (it == tenants.end()) {
            auto tenant = std::make_shared<Tenant>(name);
            tenants[name] = tenant;
            return tenant;
        }
        return it->second;
    }
private:
    std::map<std::string, std::shared_ptr<Tenant>> tenants;
};

class Apartment {
public:
    void addOccupant(const std::string& name, int room) {
        m_occupants[room] = m_registry.findByName(name);
    }

    void tenants() {
        for (const auto &i : m_occupants) {
            const int& room = i.first;
            const auto& tenant = i.second;
            std::cout << tenant->name() << " occupies room " << room << std::endl;
        }
    }
private:
    std::map<int, std::shared_ptr<Tenant>> m_occupants;
    Registry m_registry;
};

int main() {
    Apartment apartment;
    apartment.addOccupant("David", 1);
    apartment.addOccupant("Sarah", 3);
    apartment.addOccupant("George", 2);
    apartment.addOccupant("Lisa", 12);
    apartment.addOccupant("Michael", 10);
    apartment.tenants();

    return 0;
}