// File: main.cpp

#include "../inc/Server.hpp"

void handleSignal(int signal, Server &server)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        std::cout << "Received signal " << signal << ", closing server..." << std::endl;
        server.stop();
        exit(0);
    }
};

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "Usage: ./irc [config_file]" << std::endl;
        return (1);
    }
    Server server(atoi(av[1]), av[2]);
    signal(SIGINT, Server::handleSignal);
    signal(SIGTERM, Server::handleSignal);
    server.start();
    return (0);
}