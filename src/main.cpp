// File: main.cpp

#include "../inc/Server.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cout << "Usage: ./irc [config_file]" << std::endl;
        return (1);
    }
    Server server(atoi(av[1]));
    server.start();
    return (0);
}