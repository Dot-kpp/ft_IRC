#include "../../inc/commands/Names.hpp"

using std::cout;
using std::endl;

Names::Names() : command("NAMES") {}

Names::~Names() {}

Names::Names(Names const &src) {
	*this = src;
}

//bool Names::execute(Server *server, std::string args, int clientFd) {
//	cout << "You are in NAMES execute" << endl;
//	if (args.empty() || clientFd < 0) {
//		cout << "Not enough param or client doesn't exist" << endl;
//		return false;
//	}
//
//	// Parse channel names from the args parameter
//	std::istringstream iss(args);
//	std::string channelName;
//	std::vector<std::string> channelNames;
//
//	while (std::getline(iss, channelName, ',')) {
//		// Trim leading and trailing whitespaces
//		size_t start = channelName.find_first_not_of(" \t\n\r");
//		size_t end = channelName.find_last_not_of(" \t\n\r");
//		channelName = (start == std::string::npos || end == std::string::npos) ? "" : channelName.substr(start, end - start + 1);
//
//		if (!channelName.empty()) {
//			channelNames.push_back(channelName);
//		}
//	}
//
//	if (channelNames.empty()) {
//		cout << "No valid channel names provided" << endl;
//		return false;
//	}
//
//	// Iterate through each channel name and print users
//	for (size_t i = 0; i < channelNames.size(); ++i) {
//		// Find the channel by name
//		Channels* channel = server->getChannelByName(channelNames[i]);
//
//		// Check if the channel exists
//		if (channel == NULL) {
//			cout << "Channel '" << channelNames[i] << "' not found" << endl;
//		} else {
//			cout << "Users in Channel '" << channelNames[i] << "':\n" << channel->getClients();
//		}
//
//		// Add a separator between channel outputs
//		if (i < channelNames.size() - 1) {
//			cout << "-----------------------" << endl;
//		}
//	}
//
//	return true;
//}


std::string trim(const std::string& str) {
	size_t start = str.find_first_not_of(" \t\r\n");
	size_t end = str.find_last_not_of(" \t\r\n");

	if (start == std::string::npos || end == std::string::npos)
		return "";

	return str.substr(start, end - start + 1);
}

bool Names::execute(Server *server, std::string args, int clientFd) {
	std::cout << "You are in NAMES execute" << std::endl;

	if (args.empty() || clientFd < 0) {
		std::cout << "Not enough param or client doesn't exist" << std::endl;
		return false;
	}

	std::string channelName;
	std::istringstream iss(args);

	// Iterate through channel names separated by a comma
	while (std::getline(iss, channelName, ',')) {
		// Trim leading and trailing whitespaces from the channel name
		channelName = trim(channelName);

		// Get the channel object by name
		Channels *channel = server->getChannelByName(channelName);

		if (channel != nullptr) {
			std::string namesList;

			// Start of NAMES list numeric reply
			std::string replyStart = ":" + server->getServerName() + " 353 " + server->clients[clientFd].getNickName() + " = " + channelName + " :";
			send(clientFd, replyStart.c_str(), replyStart.size(), 0);

			// Iterate through clients in the channel and send their names
			const std::vector<Client *> &clientsList = channel->getClients();
			for (size_t i = 0; i < clientsList.size(); ++i) {
				// Add the appropriate channel membership symbol here, e.g., "=" for public channel
				namesList += "=" + clientsList[i]->getNickName() + " ";
			}

			// Send the NAMES list
			send(clientFd, namesList.c_str(), namesList.size(), 0);

			// End of NAMES list numeric reply
			std::string replyEnd = ":" + server->getServerName() + " 366 " + server->clients[clientFd].getNickName() + " " + channelName + " :End of /NAMES list \r\n";
			send(clientFd, replyEnd.c_str(), replyEnd.size(), 0);
		} else {
			// Channel not found
			std::string replyError = ":" + server->getServerName() + " 403 " + server->clients[clientFd].getNickName() + " " + channelName + " :No such channel \r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
		}
	}

	return true;
}
