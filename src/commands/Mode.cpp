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
	(void)clientFd;
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
	iss >> channelName >> modeChanges;

	// Find the channel by name
	Channels *channel = server->getChannelByName(channelName);

	// Check if the channel exists
	if (channel == NULL) {
		std::cout << "Channel '" << channelName << "' not found" << std::endl;
		return false;
	}

	// Process the mode changes
	for (std::string::iterator it = modeChanges.begin(); it != modeChanges.end(); ++it) {
		char mode = *it;
		switch (mode) {
			case 'i':
				cout << "Toggling channel invite only" << endl;
				cout <<"Invite only status: " << channel->getInviteOnly() << endl;
				channel->toggleInviteOnly();
				if (channel->getInviteOnly())
					cout << "Channel now invite only" << endl;
				else
					cout << "Channel no longer invite only" << endl;
				break;
			case 't':
				cout << "Toggling channel topic restriction" << endl;
				channel->toggleTopicRestriction();
				if (channel->getTopicRestriction())
					cout << "Channel now has topic restriction" << endl;
				else
					cout << "Channel no longer has topic restriction" << endl;
				break;
			case 'k':
				cout << "Toggling channel key" << endl;
				cout <<"Key status: " << channel->getHasKey() << endl;
				channel->toggleChannelKey();
				if (channel->getHasKey())
					cout << "Channel now has a key" << endl;
				else
					cout << "Channel no longer has a key" << endl;
				break;
			case 'l':
				cout << "Toggling channel user limit" << endl;
				break;
			default:
				std::cout << "Unsupported mode: " << mode << std::endl;
				break;
		}
	}

	// Send a MODE message to notify clients about the mode changes

	return true;
}


