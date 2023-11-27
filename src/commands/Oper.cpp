
#include "../../inc/commands/Oper.hpp"

Oper::Oper() : Command()
{
    this->command = "OPER";
    operMap[1] = "Super Admin";
    operMap[2] = "Admin";
    operMap[3] = "User";
}

Oper::~Oper()
{
}

bool Oper::execute(Server *server, std::string args, int clientFd)
{
    (void)server;
    (void)args;
    (void)clientFd;
    return (true);
}