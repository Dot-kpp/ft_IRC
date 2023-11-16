// File: main.cpp

#include "../inc/Server.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "Usage: ./irc [config_file]" << std::endl;
        return (1);
    }
    Server server(atoi(av[1]), av[2]);
    server.start();
    return (0);
}