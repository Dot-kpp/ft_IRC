#ifndef PART_HPP
#define PART_HPP

#include "Command.hpp"
#include <functional>

class Part : public Command {
private:
	std::string command;

public:
	Part();
	Part(Part const &src);
	virtual ~Part();

	virtual bool execute(Server *server, std::string args, int clientFd);

};

#endif