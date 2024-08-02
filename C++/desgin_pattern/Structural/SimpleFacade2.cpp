const long kBootAddress = 0x0000;
const long kBootSector = 0;
const int kSectorSize = 512;

struct CPU {
    void Freeze();
    void Jump(long position);
    void Execute();
};

struct HardDrive {
    char* Read(long lba, int size);
};

struct Memory {
    void Load(long position, char* data);
};

class ComputerFacade {
public:
    void Start() {
        cpu_.Freeze();
        memory_.Load(kBootAddress, hard_drive_.Read(kBootSector, kSectorSize));
        cpu_.Jump(kBootAddress);
        cpu_.Execute();
    }

private:
    CPU cpu_;
    Memory memory_;
    HardDrive hard_drive_;
};

int main() {
    ComputerFacade computer;
    computer.Start();
}