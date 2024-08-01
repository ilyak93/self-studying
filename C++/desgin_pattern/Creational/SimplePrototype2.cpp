#include <iostream>

enum Direction {North, South, East, West};

class MapSite {
public:
    virtual void enter() = 0;
    virtual MapSite* clone() const = 0;
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
    virtual Room* clone() const { // implements an operation for cloning itself.
        return new Room(*this);
    }
    Room& operator=(const Room&) = delete;
private:
    int roomNumber;
};

class Wall : public MapSite {
public:
    Wall() {}
    virtual void enter() {}
    virtual Wall* clone() const {
        return new Wall(*this);
    }
};

class Door : public MapSite {
public:
    Door(Room* r1 = nullptr, Room* r2 = nullptr)
            :room1(r1), room2(r2) {}
    Door(const Door& other)
            :room1(other.room1), room2(other.room2) {}
    virtual void enter() {}
    virtual Door* clone() const {
        return new Door(*this);
    }
    virtual void initialize(Room* r1, Room* r2) {
        room1 = r1;
        room2 = r2;
    }
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
    virtual Maze* clone() const {
        return new Maze(*this);
    }
    virtual ~Maze() = default;
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

class MazePrototypeFactory : public MazeFactory {
public:
    MazePrototypeFactory(Maze* m, Wall* w, Room* r, Door* d)
            :prototypeMaze(m), prototypeRoom(r),
             prototypeWall(w), prototypeDoor(d) {}
    virtual Maze* makeMaze() const {
        // creates a new object by asking a prototype to clone itself.
        return prototypeMaze->clone();
    }
    virtual Room* makeRoom(int) const {
        return prototypeRoom->clone();
    }
    virtual Wall* makeWall() const {
        return prototypeWall->clone();
    }
    virtual Door* makeDoor(Room* r1, Room* r2) const {
        Door* door = prototypeDoor->clone();
        door->initialize(r1, r2);
        return door;
    }
    MazePrototypeFactory(const MazePrototypeFactory&) = delete;
    MazePrototypeFactory& operator=(const MazePrototypeFactory&) = delete;
private:
    Maze* prototypeMaze;
    Room* prototypeRoom;
    Wall* prototypeWall;
    Door* prototypeDoor;
};

// If createMaze is parameterized by various prototypical room, door, and wall objects, which it then copies and adds to the maze, then you can change the maze's composition by replacing these prototypical objects with different ones. This is an example of the Prototype (133) pattern.

class MazeGame {
public:
    Maze* createMaze(MazePrototypeFactory& m) {
        Maze* aMaze = m.makeMaze();
        Room* r1 = m.makeRoom(1);
        Room* r2 = m.makeRoom(2);
        Door* theDoor = m.makeDoor(r1, r2);
        aMaze->addRoom(r1);
        aMaze->addRoom(r2);
        r1->setSide(North, m.makeWall());
        r1->setSide(East, theDoor);
        r1->setSide(South, m.makeWall());
        r1->setSide(West, m.makeWall());
        r2->setSide(North, m.makeWall());
        r2->setSide(East, m.makeWall());
        r2->setSide(South, m.makeWall());
        r2->setSide(West, theDoor);
        return aMaze;
    }
};

int main() {
    MazeGame game;
    MazePrototypeFactory simpleMazeFactory(new Maze, new Wall, new Room, new Door);
    game.createMaze(simpleMazeFactory);
}