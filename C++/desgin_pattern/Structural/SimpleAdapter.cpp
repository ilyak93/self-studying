#include <iostream>
#include <memory>

// Target interface
class IUsbPhone {
public:
    virtual void connectUsb() = 0;
    virtual void recharge() = 0;
    virtual ~IUsbPhone() = default;
};

// Adaptee interface
class ILightningPhone {
public:
    virtual void connectLightning() = 0;
    virtual void recharge() = 0;
    virtual ~ILightningPhone() = default;
};

// Concrete Adaptee
class IPhone : public ILightningPhone {
private:
    bool isConnected = false;

public:
    void connectLightning() override {
        isConnected = true;
        std::cout << "Lightning connected to iPhone." << std::endl;
    }

    void recharge() override {
        if (isConnected) {
            std::cout << "iPhone recharging..." << std::endl;
        } else {
            std::cout << "Connect Lightning cable first." << std::endl;
        }
    }
};

// Concrete Target
class AndroidPhone : public IUsbPhone {
private:
    bool isConnected = false;

public:
    void connectUsb() override {
        isConnected = true;
        std::cout << "USB connected to Android phone." << std::endl;
    }

    void recharge() override {
        if (isConnected) {
            std::cout << "Android phone recharging..." << std::endl;
        } else {
            std::cout << "Connect USB cable first." << std::endl;
        }
    }
};

// Adapter
class LightningToUsbAdapter : public IUsbPhone {
private:
    std::shared_ptr<ILightningPhone> lightningPhone;

public:
    LightningToUsbAdapter(std::shared_ptr<ILightningPhone> phone) : lightningPhone(phone) {}

    void connectUsb() override {
        std::cout << "Adapter: Converting USB to Lightning." << std::endl;
        lightningPhone->connectLightning();
    }

    void recharge() override {
        lightningPhone->recharge();
    }
};

// Client code
void rechargePhone(IUsbPhone& phone) {
    phone.connectUsb();
    phone.recharge();
}

int main() {
    // Using Android phone
    AndroidPhone androidPhone;
    std::cout << "Recharging Android phone:" << std::endl;
    rechargePhone(androidPhone);

    std::cout << std::endl;

    // Using iPhone with adapter
    auto iPhone = std::make_shared<IPhone>();
    LightningToUsbAdapter adapter(iPhone);
    std::cout << "Recharging iPhone with adapter:" << std::endl;
    rechargePhone(adapter);

    return 0;
}