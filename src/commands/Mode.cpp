#include "../../inc/commands/Mode.hpp"

using std::cout;
using std::endl;
using std::string;

Mode::Mode() : command("MODE") {}

Mode::~Mode() {}

Mode::Mode(Mode const &src) {
	*this = src;
}

bool Mode::execute(Server *server, std::string args, int clientFd) {

	// Check if enough parameters are provided
	if (args.empty()) {
		std::cout << "Not enough parameters for MODE command" << std::endl;
		return false;
	}

	// Parse the arguments to get the channel name and mode changes
	std::istringstream iss(args);
	std::string channelName;
	std::string modeChanges;
	std::string target;

	iss >> channelName >> modeChanges >> target;

	// Find the client by nickname in the current channel
	Client *client = server->getClientByFd(clientFd);

	Client *targetClient = server->getClientByNickname(target);

	// Find the channel by name
	Channels *channel = server->getChannelByName(channelName);

	// Check if the channel exists
	if (channel == nullptr) {
		std::string replyError = ":" + server->getServerName() + " 403 " + server->clients[clientFd].getNickName() + " " + channelName + " :No such channel \r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
		return false;
	}

	bool settingMode = true;

	// SYNTAXE : MODE #exampleChannel [+itkol] [args]
	for (std::string::iterator it = modeChanges.begin(); it != modeChanges.end(); ++it) {
		char mode = *it;

		if (mode == '+') {
			settingMode = true;
			continue;
		} else if (mode == '-') {
			settingMode = false;
			continue;
		}

		if (channel->isOperator(client)) {
			switch (mode) {
				case 'i':
					// MODE #exampleChannel +i
					if (settingMode) {
						channel->setInviteOnly(true);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " +i \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
					} else {
						channel->setInviteOnly(false);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " -i \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
					}
					break;

				case 't':
					// MODE #exampleChannel +t
					if (settingMode) {
						channel->setTopicRestriction(true);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " +t \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
					} else {
						channel->setTopicRestriction(false);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " -t \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
					}
					break;

				case 'o':
					if (targetClient != nullptr) {
						if (settingMode) {
							channel->promoteUser(targetClient);
							std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " +o \r\n";
							send(clientFd, replyError.c_str(), replyError.size(), 0);
							server->broadcastToChannel(channelName, replyError, clientFd, client->getNickName());
						} else {
							channel->demoteUser(targetClient);
							std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " -o \r\n";
							send(clientFd, replyError.c_str(), replyError.size(), 0);
							server->broadcastToChannel(channelName, replyError, clientFd, client->getNickName());
						}
					} else {
						std::string replyError = ":" + server->getServerName() + " 441 " + server->clients[clientFd].getNickName() + " " + target + " :They aren't on that channel \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
					}
					break;

				case 'k':
					// MODE #secretChannel +k mySecretKey
					if (settingMode) {
						channel->setChannelKeyRestriction(true);
						if(target.empty()){
							std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " MODE " + channelName + " +k :Not enough parameters \r\n";
							send(clientFd, replyError.c_str(), replyError.size(), 0);
							return false;
						}
						target.erase(std::remove(target.begin(), target.end(), '\n'), target.end());
						target.erase(std::remove(target.begin(), target.end(), '\r'), target.end());
						channel->setKey(target);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " +k \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
						server->broadcastToChannel(channelName, replyError, clientFd, client->getNickName());
					} else {
						channel->setChannelKeyRestriction(false);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " -k \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
						server->broadcastToChannel(channelName, replyError, clientFd, client->getNickName());
					}
					break;

				case 'l':
					// MODE #exampleChannel +l [userLimit]
					if (settingMode){
						if(target.empty()){
							std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " MODE " + channelName + " +l :Not enough parameters \r\n";
							send(clientFd, replyError.c_str(), replyError.size(), 0);
							return false;
						}
						channel->setUserRestriction(true);
						try {
							int userLimit = std::stoi(target);
							channel->setUserLimit(userLimit);
						} catch (std::invalid_argument& e) {
							std::string replyError = ":" + client->getNickName() + " MODE " + channelName + " +l :Forbidden char in userlimit \r\n";
							send(clientFd, replyError.c_str(), replyError.size(), 0);
							return false;
						}
						channel->setUserLimit(stoi(target));
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " +l \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
						server->broadcastToChannel(channelName, replyError, clientFd, client->getNickName());
					} else {
						channel->setUserRestriction(false);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " -l \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
						server->broadcastToChannel(channelName, replyError, clientFd, client->getNickName());
					}
					break;

				default:
					std::string replyError = ":" + server->getServerName() + " 472 " + server->clients[clientFd].getNickName() + " " + channelName + " :is unknown mode to me\r\n";
					send(clientFd, replyError.c_str(), replyError.size(), 0);
					break;
			}
		}
		else {
			std::string replyError = ":" + server->getServerName() + " 482 " + server->clients[clientFd].getNickName() + " " + channelName + " :You're not channel operator\r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
		}
	}

	return true;
}
