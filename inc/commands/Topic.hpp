#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "Command.hpp"

class Topic : public Command {
private:
	std::string command;

public:
	Topic();
	Topic(Topic const &src);
	virtual ~Topic();

	virtual bool execute(Server *server, std::string args, int clientFd);
	std::string trim(const std::string& str);

};

#endif