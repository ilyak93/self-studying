#include <iostream>

enum Direction {North, South, East, West};

class MapSite {
public:
    virtual void enter() = 0;
    virtual ~MapSite() = default;
};

class Room : public MapSite {
public:
    Room() :roomNumber(0) {}
    Room(int n) :roomNumber(n) {}
    void setSide(Direction d, MapSite* ms) {
        std::cout << "Room::setSide " << d << ' ' << ms << '\n';
    }
    virtual void enter() {}
    Room(const Room&) = delete; // rule of three
    Room& operator=(const Room&) = delete;
private:
    int roomNumber;
};

class Wall : public MapSite {
public:
    Wall() {}
    virtual void enter() {}
};

class Door : public MapSite {
public:
    Door(Room* r1 = nullptr, Room* r2 = nullptr)
            :room1(r1), room2(r2) {}
    virtual void enter() {}
    Door(const Door&) = delete; // rule of three
    Door& operator=(const Door&) = delete;
private:
    Room* room1;
    Room* room2;
};

class Maze {
public:
    void addRoom(Room* r) {
        std::cout << "Maze::addRoom " << r << '\n';
    }
    Room* roomNo(int) const {
        return nullptr;
    }
};

class MazeFactory {
public:
    MazeFactory() = default;
    virtual ~MazeFactory() = default;

    virtual Maze* makeMaze() const {
        return new Maze;
    }
    virtual Wall* makeWall() const {
        return new Wall;
    }
    virtual Room* makeRoom(int n) const {
        return new Room(n);
    }
    virtual Door* makeDoor(Room* r1, Room* r2) const {
        return new Door(r1, r2);
    }
};

// If createMaze is passed an object as a parameter to use to create rooms, walls, and doors, then you can change the classes of rooms, walls, and doors by passing a different parameter. This is an example of the Abstract Factory (99) pattern.

class MazeGame {
public:
    Maze* createMaze(MazeFactory& factory) {
        Maze* aMaze = factory.makeMaze();
        Room* r1 = factory.makeRoom(1);
        Room* r2 = factory.makeRoom(2);
        Door* aDoor = factory.makeDoor(r1, r2);
        aMaze->addRoom(r1);
        aMaze->addRoom(r2);
        r1->setSide(North, factory.makeWall());
        r1->setSide(East, aDoor);
        r1->setSide(South, factory.makeWall());
        r1->setSide(West, factory.makeWall());
        r2->setSide(North, factory.makeWall());
        r2->setSide(East, factory.makeWall());
        r2->setSide(South, factory.makeWall());
        r2->setSide(West, aDoor);
        return aMaze;
    }
};

int main() {
    MazeGame game;
    MazeFactory factory;
    game.createMaze(factory);
}