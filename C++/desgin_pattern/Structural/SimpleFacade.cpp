
#include <iostream>

// Subsystem 1
class Engine {
public:
    void Start()
    {
        std::cout << "Engine started" << std::endl;
    }

    void Stop()
    {
        std::cout << "Engine stopped" << std::endl;
    }
};

// Subsystem 2
class Lights {
public:
    void TurnOn() { std::cout << "Lights on" << std::endl; }

    void TurnOff()
    {
        std::cout << "Lights off" << std::endl;
    }
};

// Facade
class Car {
private:
    Engine engine;
    Lights lights;

public:
    void StartCar()
    {
        engine.Start();
        lights.TurnOn();
        std::cout << "Car is ready to drive" << std::endl;
    }

    void StopCar()
    {
        lights.TurnOff();
        engine.Stop();
        std::cout << "Car has stopped" << std::endl;
    }
};

int main()
{
    // Using the Facade to start and stop the car
    Car car;
    car.StartCar();
    // Simulate some driving
    car.StopCar();
    return 0;
}
