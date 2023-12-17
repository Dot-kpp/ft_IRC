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
	std::cout << "You are in MODE execute" << std::endl;

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
//	cout << "args: " << args << endl;
	iss >> channelName >> modeChanges >> target;

	// Find the client by nickname in the current channel
	Client *client = server->getClientByFd(clientFd);

	Client *targetClient = server->getClientByNickname(target);

	// Find the channel by name
	cout << "channelName: " << channelName << endl;
	cout << "channelName size: " << channelName.size() << endl;
	Channels *channel = server->getChannelByName(channelName);
	// Check if the channel exists
	if (channel == nullptr) {
		cout << "here"  << endl;
		std::string replyError = ":" + server->getServerName() + " 403 " + server->clients[clientFd].getNickName() + " " + channelName + " :No such channel \r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
		return false;
	}

	bool settingMode = true;

	// Process the mode changes
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
						cout << "Channel now invite only" << endl;
					} else {
						channel->setInviteOnly(false);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " -i \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
						cout << "Channel no longer invite only" << endl;
					}
					break;

				case 't':
					// MODE #exampleChannel +t
					if (settingMode) {
						channel->setTopicRestriction(true);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " +t \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
						cout << "Channel now has topic restriction" << endl;
					} else {
						channel->setTopicRestriction(false);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " -t \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
						cout << "Channel no longer has topic restriction" << endl;
					}
					break;

				case 'o':
					if (targetClient != nullptr) {
						if (settingMode) {
							cout << "Promoting " << targetClient->getNickName() << " to operator" << endl;
							channel->promoteUser(targetClient);
							std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " +o \r\n";
							send(clientFd, replyError.c_str(), replyError.size(), 0);
						} else {
							std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " -o \r\n";
							send(clientFd, replyError.c_str(), replyError.size(), 0);
							channel->demoteUser(targetClient);
						}
					} else {
						std::string replyError = ":" + server->getServerName() + " 441 " + server->clients[clientFd].getNickName() + " " + target + " :They aren't on that channel \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
						std::cout << "No target user provided" << std::endl;
					}
				break;

				case 'k':
					// MODE #secretChannel +k mySecretKey
//					channel->toggleChannelKey();
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
					} else {
						channel->setChannelKeyRestriction(false);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " -k \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
					}
					break;

				case 'l':
					// MODE #exampleChannel +l [userLimit]
					channel->toggleUserLimit();
					channel->setUserLimit(stoi(target));
					if (channel->getHasLimit())
						cout << "Channel now has a user limit" << endl;
					else
						cout << "Channel no longer has a user limit" << endl;
					break;

				default:
					std::cout << "Unsupported mode: " << mode << std::endl;
					break;
			}
		}
		else {
			std::string replyError = ":" + server->getServerName() + " 482 " + server->clients[clientFd].getNickName() + " " + channelName + " :You're not channel operator\r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			std::cout << "You are not an operator" << std::endl;
		}
	}

	// Send a MODE message to notify clients about the mode changes

	return true;
}
