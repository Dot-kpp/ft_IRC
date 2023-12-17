// File: main.cpp

#include "../inc/server/Server.hpp"

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
        std::cout << "Usage: ./irc [port] [password]" << std::endl;
        return (1);
    }

	int port;
	if (std::string(av[1]).find_first_not_of("0123456789") == std::string::npos) {
		try {
			port = std::stoi(av[1]);
		} catch (std::invalid_argument& e) {
			std::cerr << "Port must be a valid number" << std::endl;
			return (1);
		} catch (std::out_of_range& e) {
			std::cerr << "Port number is too large" << std::endl;
			return (1);
		} catch (std::length_error& e) {
			std::cerr << "Port number is out of range for int" << std::endl;
			return (1);
		}
	} else {
		std::cerr << "Port must be a number" << std::endl;
		return (1);
	}
    Server server(port, av[2]);
    signal(SIGINT, Server::handleSignal);
    signal(SIGTERM, Server::handleSignal);
    server.start();
    return (0);
}