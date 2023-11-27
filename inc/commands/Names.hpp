#ifndef NAMES_HPP
#define NAMES_HPP

#include "Command.hpp"

class Names : public Command {
private:
	std::string command;

public:
	Names();
	Names(Names const &src);
	virtual ~Names();

	virtual bool execute(Server *server, std::string args, int clientFd);

};

#endif