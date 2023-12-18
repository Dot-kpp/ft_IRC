#ifndef NAMES_HPP
#define NAMES_HPP

#include "Command.hpp"
#include <map>
#include <vector>

class Names : public Command {
private:
	std::string command;

public:
	Names();
	Names(Names const &src);
	virtual ~Names();

	virtual bool execute(Server *server, std::string args, int clientFd);
	std::string trim(const std::string& str);

};

#endif