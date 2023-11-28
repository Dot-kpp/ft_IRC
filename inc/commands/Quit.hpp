
#ifndef QUIT_HPP
#define QUIT_HPP

#include "Command.hpp"

class Quit : public Command
{
public:
    Quit();
    ~Quit();
    bool execute(Server &server, std::string args, int clientSocket);
};

#endif // QUIT_HPP