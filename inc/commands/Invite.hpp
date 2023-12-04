#ifndef INVITE_HPP
#define INVITE_HPP

#include "Command.hpp"

class Invite : public Command {
private:
	std::string command;

public:
	Invite();
	Invite(Invite const &src);
	virtual ~Invite();

	virtual bool execute(Server *server, std::string args, int clientFd);
};

#endif