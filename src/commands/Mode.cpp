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
	cout << "args: " << args << endl;
	iss >> channelName >> modeChanges >> target;

	// Find the client by nickname in the current channel
	Client *targetClient = server->getClientByFd(clientFd);

	// Find the channel by name
	Channels *channel = server->getChannelByName(channelName);

	// Check if the channel exists
	if (channel == nullptr) {
		std::string replyError = ":" + server->getServerName() + " 403 " + server->clients[clientFd].getNickName() + " " + channelName + " :No such channel \r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
		return false;
	}

	bool isSettingMode = true; // Default to setting mode

	// Process the mode changes
	// SYNTAXE : MODE #exampleChannel [+itkol] [args]
	for (std::string::iterator it = modeChanges.begin(); it != modeChanges.end(); ++it) {
		char mode = *it;

		if (mode == '+') {
			isSettingMode = true;
			continue;
		} else if (mode == '-') {
			isSettingMode = false;
			continue;
		}

		std::string plus = "true";
		std::string minus = "false";
		if (channel->isOperator(targetClient)) {
			switch (mode) {
				case 'i':
					// MODE #exampleChannel +i
					if (isSettingMode) {
						channel->setInviteOnly(plus);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " +i \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
						cout << "Channel now invite only" << endl;
					} else {
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " -i \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
						cout << "Channel no longer invite only" << endl;
					}
					break;

				case 't':
					// MODE #exampleChannel +t
					if (isSettingMode) {
						channel->setTopicRestriction(plus);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " +t \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
						cout << "Channel now has topic restriction" << endl;
					} else {
						channel->setTopicRestriction(minus);
						std::string replyError = ":" + server->clients[clientFd].getNickName() + " MODE " + channelName + " -t \r\n";
						send(clientFd, replyError.c_str(), replyError.size(), 0);
						cout << "Channel no longer has topic restriction" << endl;
					}
					break;

				case 'o':
					if (targetClient != nullptr) {
						if (isSettingMode) {
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
					channel->toggleChannelKey();
					channel->setKey(target); //need to do it by parse args

					if (channel->getHasKey()) {
						cout << "Channel now has a key" << endl;
						cout << "Key: " << channel->getKey() << endl;
					}
					else
						cout << "Channel no longer has a key" << endl;
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
