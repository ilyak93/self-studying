#include "Server.hpp"

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    Server server(args);
    server.run();
    return 0;
}