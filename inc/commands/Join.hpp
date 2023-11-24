#ifndef JOIN_HPP
#define JOIN_HPP

#include "Command.hpp"

class Join : public Command
{
public:
    Join();
    virtual ~Join();

    virtual bool execute(std::string args, int clientFd) override;
};

#endif // JOIN_HPP