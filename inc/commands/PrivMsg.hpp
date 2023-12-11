#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "Command.hpp"

class PrivMsg : public Command {
private:
	std::string command;

public:
	PrivMsg();
	PrivMsg(PrivMsg const &src);
	virtual ~PrivMsg();

	virtual bool execute(Server *server, std::string args, int clientFd);
};

#endif