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
	// SYNTAXE : MODE #exampleChannel [+itkol] [args
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

				case 't':
					// MODE #exampleChannel +t
					cout << "Toggling channel topic restriction" << endl;
					channel->toggleTopicRestriction();
					if (channel->getTopicRestriction())
						cout << "Channel now has topic restriction" << endl;
					else
						cout << "Channel no longer has topic restriction" << endl;
					break;

				case 'o':
//					cout << "Toggling channel operator" << endl;
//					cout << "Operator target: " << targetClient->getUserName() << endl;
					cout << "Operator role (before): " << channel->getUserRole(targetClient) << endl;
					if (targetClient != nullptr) {
						if (isSettingMode)
							channel->promoteUser(targetClient);
						else
							channel->demoteUser(targetClient);
					} else {
						std::cout << "No target user provided" << std::endl;
					}
					cout << "Operator after (before): " << channel->getUserRole(targetClient) << endl;
					break;

				case 'k':
					// MODE #secretChannel +k mySecretKey
					cout << "target: " << target << endl;
					cout << "Toggling channel key" << endl;
					cout << "Key status: " << channel->getHasKey() << endl;
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
					cout << "Toggling channel user limit" << endl;
					channel->toggleUserLimit();
					channel->setUserLimit(stoi(target)); //need to do it by parse args
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



//bool Mode::execute(Server *server, std::string args, int clientFd) {
//	(void)clientFd;
//	std::cout << "You are in MODE execute" << std::endl;
//
//	// Check if enough parameters are provided
//	if (args.empty()) {
//		std::cout << "Not enough parameters for MODE command" << std::endl;
//		return false;
//	}
//
//	// Parse the arguments to get the channel name and mode changes
//	std::istringstream iss(args);
//	std::string name;
//	std::string modeChanges;
//	std::string targetUser;
//
//	iss >> name >> modeChanges >> targetUser;
//
//	bool isChannel = (!name.empty() && name[0] == '#');
//
//	cout << "Channel name: " << name << endl;
//	cout << "Mode changes: " << modeChanges << endl;
//
//	if (isChannel) {
//		// Remove '#' from the channel name
//		std::string channelName = name.substr(1);
//
//		// Find the channel by name
//		Channels *channel = server->getChannelByName(channelName);
//
//		// Check if the channel exists
//		if (channel == NULL) {
//		std::cout << "Channel '" << channelName << "' not found" << std::endl;
//		return false;
//		}
//
//		bool isSettingMode = true; // Default to setting mode
//
//		// Process the mode changes
//		// SYNTAXE : MODE #exampleChannel [+itkol] [args
//		for (std::string::iterator it = modeChanges.begin(); it != modeChanges.end(); ++it) {
//			char mode = *it;
//
//			if (mode == '+') {
//				isSettingMode = true;
//				continue;
//			} else if (mode == '-') {
//				isSettingMode = false;
//				continue;
//			}
//
//			switch (mode) {
//				case 'i':
//					// MODE #exampleChannel +i
//
//					// add a check if the user is an operator
//					// add an Invite list (vector of users), and a function to add users to the list
//					// add a check if the user is in the Invite list
//					// when a user is added to the Invite list, send him a message
//					// when a user is removed from the channel, remove him from the Invite list
//					// when an Invite only is changed to false, clear the Invite list
//					// when a user is added to the channel, remove him from the Invite list
//
//					cout << "Toggling channel invite only" << endl;
//					cout << "Invite only status: " << channel->getInviteOnly() << endl;
//					channel->toggleInviteOnly();
//					if (channel->getInviteOnly())
//						cout << "Channel now invite only" << endl;
//					else
//						cout << "Channel no longer invite only" << endl;
//					break;
//
//
//				case 'k':
//					// MODE #secretChannel +k mySecretKey
//					cout << "Toggling channel key" << endl;
//					cout << "Key status: " << channel->getHasKey() << endl;
//					channel->toggleChannelKey();
//					channel->setKey("test"); //need to do it by parse args
//
//					if (channel->getHasKey())
//						cout << "Channel now has a key" << endl;
//					else
//						cout << "Channel no longer has a key" << endl;
//					break;
//
//				case 'o':
//					// MODE #exampleChannel +o exampleUser
////					iss >> targetUser; // Parse the next argument as the target user's nickname
//					if (!targetUser.empty()){
//						if (isSettingMode)
//							channel->promoteUser(targetUser);
//						else
//							channel->demoteUser(targetUser);
//					}
//					else
//						std::cout << "No target user provided" << std::endl;
//					break;
//
//				case 'l':
//					// MODE #exampleChannel +l [userLimit]
//					cout << "Toggling channel user limit" << endl;
//					channel->toggleUserLimit();
//					channel->setUserLimit(5); //need to do it by parse args
//					if (channel->getUserLimit())
//						cout << "Channel now has a user limit" << endl;
//					else
//						cout << "Channel no longer has a user limit" << endl;
//					break;
//
//				default:
//					std::cout << "Unsupported mode: " << mode << std::endl;
//					break;
//			}
//		}
//	}
//
//	// Send a MODE message to notify clients about the mode changes
//
//	return true;
//}
