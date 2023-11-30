#ifndef JOIN_HPP
#define JOIN_HPP

#include "Command.hpp"

class Join : public Command {
private:
	std::string command;

public:
	Join();
	Join(Join const &src);
	virtual ~Join();

	virtual bool execute(Server *server, std::string args, int clientFd);

};

#endif