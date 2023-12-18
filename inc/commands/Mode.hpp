#ifndef MODE_HPP
#define MODE_HPP

#include "Command.hpp"

class Mode : public Command {
private:
	std::string command;

public:
	Mode();
	Mode(Mode const &src);
	virtual ~Mode();

	virtual bool execute(Server *server, std::string args, int clientFd);
};

#endif