#ifndef NAMES_HPP
#define NAMES_HPP

#include "Command.hpp"

class Names : public Command {
public:
	Names();
	Names(Names const &src);
	~Names();

	bool execute(std::string args, int clientFd);

private:

};

#endif