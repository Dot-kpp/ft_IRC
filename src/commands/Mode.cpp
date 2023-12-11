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
	std::string name;
	std::string modeChanges;
	std::string target;

	iss >> name >> modeChanges >> target;

	// Find the client by nickname in the current channel
	Client *targetClient = server->getClientByFd(clientFd);

	cout << "Channel name: " << name << endl;
	cout << "Mode changes: " << modeChanges << endl;

	// Remove '#' from the channel name
	std::string channelName = name.substr(1);
	cout << "CHANNEL NAME: " << channelName << endl;

	// Find the channel by name
	Channels *channel = server->getChannelByName(channelName);

	// Check if the channel exists
	if (channel == nullptr) {
		std::cout << "Channel '" << channelName << "' not found" << std::endl;
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

		if (channel->isOperator(targetClient)) {
			switch (mode) {
				case 'i':
					// MODE #exampleChannel +i
//					cout << "Toggling channel invite only" << endl;
//					cout << "Invite only status: " << channel->getInviteOnly() << endl;
					channel->toggleInviteOnly();
					if (channel->getInviteOnly())
						cout << "Channel now invite only" << endl;
					else
						cout << "Channel no longer invite only" << endl;
					break;

				case 't':
					// MODE #exampleChannel +t
//					cout << "Toggling channel topic restriction" << endl;
					channel->toggleTopicRestriction();
					if (channel->getTopicRestriction())
						cout << "Channel now has topic restriction" << endl;
					else
						cout << "Channel no longer has topic restriction" << endl;
					break;

				case 'o':
//					cout << "Operator role (before): " << channel->getUserRole(targetClient) << endl;
					if (targetClient != nullptr) {
						if (isSettingMode)
							channel->promoteUser(targetClient);
						else
							channel->demoteUser(targetClient);
					} else {
						std::cout << "No target user provided" << std::endl;
					}
//					cout << "Operator after (before): " << channel->getUserRole(targetClient) << endl;
					break;

				case 'k':
					// MODE #secretChannel +k mySecretKey
//					cout << "target: " << target << endl;
//					cout << "Toggling channel key" << endl;
//					cout << "Key status: " << channel->getHasKey() << endl;
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
//					cout << "Toggling channel user limit" << endl;
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
			std::cout << "You are not an operator" << std::endl;
		}
	}

	// Send a MODE message to notify clients about the mode changes

	return true;
}
