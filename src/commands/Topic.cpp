#include "../../inc/commands/Topic.hpp"

using std::cout;
using std::endl;
using std::string;

Topic::Topic() : command("NAMES") {}

Topic::~Topic() {}

Topic::Topic(Topic const &src) {
	*this = src;
}

std::string Topic::trim(const std::string& str) {
	size_t start = str.find_first_not_of(" \t\r\n");
	size_t end = str.find_last_not_of(" \t\r\n");

	if (start == std::string::npos || end == std::string::npos)
		return "";

	return str.substr(start, end - start + 1);
}

//bool Topic::execute(Server *server, std::string args, int clientFd) {
////	(void)server;
////	(void)args;
////	(void)clientFd;
//	cout << "You are in TOPIC execute" << endl; // NEED To remove
//
//
//
//	return true;
//}

bool Topic::execute(Server *server, std::string args, int clientFd) {
	std::cout << "You are in TOPIC execute" << std::endl;

	if (args.empty() || clientFd < 0) {
		std::cout << "Not enough param or client doesn't exist" << std::endl;
		return false;
	}

	std::string channelName;
	std::istringstream iss(args);

	// Get the channel name
	std::getline(iss, channelName, ' ');

	// Trim leading and trailing whitespaces from the channel name
	channelName = trim(channelName);

	// Get the channel object by name
	Channels *channel = server->getChannelByName(channelName);

	if (channel != nullptr) {
		// Get the remaining argument after the channel name
		std::string topicArg;
		std::getline(iss, topicArg);

		// Trim leading and trailing whitespaces from the topic argument
		topicArg = trim(topicArg);

		if (topicArg.empty()) {
			// No topic argument provided, check and send the current topic
			std::string replyTopic = ":" + server->getServerName() + " 332 " + server->clients[clientFd].getNickName() + " #" + channelName + " :" + channel->getTopic() + "\r\n";
			send(clientFd, replyTopic.c_str(), replyTopic.size(), 0);
		} else {
			// Set or clear the topic based on the argument
			if (topicArg == ":") {
				// Clear the topic
				channel->setTopic("");
				std::string replyTopic = ":" + server->getServerName() + " 331 " + server->clients[clientFd].getNickName() + " #" + channelName + " :No topic is set\r\n";
				send(clientFd, replyTopic.c_str(), replyTopic.size(), 0);
			} else {
				// Set the topic
				topicArg = topicArg.substr(1); // Remove the leading ':'
				channel->setTopic(topicArg);
				std::string replyTopic = ":" + server->getServerName() + " 332 " + server->clients[clientFd].getNickName() + " #" + channelName + " :" + channel->getTopic() + "\r\n";
				send(clientFd, replyTopic.c_str(), replyTopic.size(), 0);
			}
		}
	} else {
		// Channel not found
		std::string replyError = ":" + server->getServerName() + " 403 " + server->clients[clientFd].getNickName() + " " + channelName + " :No such channel \r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
	}

	return true;
}

