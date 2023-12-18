#include "../../inc/commands/Topic.hpp"

using std::cout;
using std::endl;
using std::string;

Topic::Topic() : command("TOPIC") {}

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

bool Topic::execute(Server *server, std::string args, int clientFd) {
	if (args.empty() || clientFd < 0) {
		std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " TOPIC :Not enough parameters \r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
		std::cout << "Not enough param or client doesn't exist" << std::endl;
		return false;
	}

	std::istringstream iss(args);
	std::string channelInfo;

	Client *targetClient = server->getClientByNickname(server->clients[clientFd].getNickName());
	while(std::getline(iss, channelInfo, ',')){
		size_t spacePos = channelInfo.find(' ');

		std::string channelName;
		std::string providedTopic;

		if (spacePos != std::string::npos) {
			// Split channelInfo into channelName and providedKey
			channelName = channelInfo.substr(0, spacePos);
			providedTopic = channelInfo.substr(spacePos + 1);

			providedTopic.erase(std::remove(providedTopic.begin(), providedTopic.end(), '\n'), providedTopic.end());
			providedTopic.erase(std::remove(providedTopic.begin(), providedTopic.end(), '\r'), providedTopic.end());

		} else {
			// No space found, use channelInfo as channelName
			channelName = channelInfo;
			providedTopic.clear();
		}

		// Remove \n and \r characters from channelName
		channelName.erase(std::remove(channelName.begin(), channelName.end(), '\n'), channelName.end());
		channelName.erase(std::remove(channelName.begin(), channelName.end(), '\r'), channelName.end());
		if (channelName.front() != '#') {
			std::string replyError = ":" + server->getServerName() + " 476 " + channelName + " :Bad Channel Mask\r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			return false;
		}
		// check if there are # after the first position of the string
		if (channelName.find('#', 1) != std::string::npos)
			return false;

		Channels *channel = server->getChannelByName(channelName);

		if (providedTopic.empty()) {
		std::string replyTopic = ":" + server->getServerName() + " 332 " + server->clients[clientFd].getNickName() + " " + channelName + " :" + channel->getTopic() + "\r\n";
		send(clientFd, replyTopic.c_str(), replyTopic.size(), 0);
		} else {
			if(channel->getTopicRestriction() && !channel->isOperator(targetClient)) {
				std::string replyError = ":" + server->getServerName() + " 482 " + server->clients[clientFd].getNickName() + " " + channelName + " :You're not channel operator\r\n";
				send(clientFd, replyError.c_str(), replyError.size(), 0);
				return false;
			}

			// Add a rule to limit the topic length to 420 characters
			if (providedTopic.length() > 42) {
				std::string replyError = ":" + server->getServerName() + " 442 " + server->clients[clientFd].getNickName() + " " + channelName + " :Topic is too long (max 420 characters)\r\n";
				send(clientFd, replyError.c_str(), replyError.size(), 0);
				return false;
			}

			providedTopic = providedTopic.substr(1);
			channel->setTopic(providedTopic);
			std::string replyTopic = ":" + server->getServerName() + " 332 " + server->clients[clientFd].getNickName() + " " + channelName + " :" + channel->getTopic() + "\r\n";
			send(clientFd, replyTopic.c_str(), replyTopic.size(), 0);
		}

	}
	return true;
}
