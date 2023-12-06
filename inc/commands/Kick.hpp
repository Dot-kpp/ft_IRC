#ifndef KICK_HPP
#define KICK_HPP

#include "Command.hpp"
#include <functional>

class Kick : public Command {
private:
	std::string command;

public:
	Kick();
	Kick(Kick const &src);
	virtual ~Kick();

	virtual bool execute(Server *server, std::string args, int clientFd);

};

#endif