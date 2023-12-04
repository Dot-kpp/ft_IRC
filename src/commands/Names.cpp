#include "../../inc/commands/Names.hpp"

using std::cout;
using std::endl;
using std::string;

Names::Names() : command("NAMES") {}

Names::~Names() {}

Names::Names(Names const &src) {
	*this = src;
}

std::string Names::trim(const std::string& str) {
	size_t start = str.find_first_not_of(" \t\r\n");
	size_t end = str.find_last_not_of(" \t\r\n");

	if (start == std::string::npos || end == std::string::npos)
		return "";

	return str.substr(start, end - start + 1);
}

bool Names::execute(Server *server, std::string args, int clientFd) {
	//NEED add user's prefix when sending the list of names

	std::cout << "You are in NAMES execute" << std::endl; // NEED To remove

	if (args.empty() || clientFd < 0) {
		cout << "Not enough param or client doesn't exist" << endl;
		return false;
	}

	string channelName;
	std::istringstream iss(args);

	// Iterate through channel names separated by a comma
	while (getline(iss, channelName, ',')) {
		// Trim leading and trailing whitespaces from the channel name
		channelName = trim(channelName);

		// Get the channel object by name
		Channels *channel = server->getChannelByName(channelName);

		if (channel != nullptr) {
			string namesList;

			// Start of NAMES list numeric reply
			string replyStart = ":" + server->getServerName() + " 353 " + server->clients[clientFd].getNickName() + " = #" + channelName + " : ";
			send(clientFd, replyStart.c_str(), replyStart.size(), 0);

			// Iterate through clients in the channel and send their names
			const std::vector<Client *> &clientsList = channel->getClients();
			for (size_t i = 0; i < clientsList.size(); ++i) {
				// Add the appropriate channel membership symbol here, e.g., "=" for public channel
				namesList += clientsList[i]->getNickName() + " ";
			}

			// Send the NAMES list
			send(clientFd, namesList.c_str(), namesList.size(), 0);

			// End of NAMES list numeric reply
			string replyEnd = "\r\n:" + server->getServerName() + " 366 " + server->clients[clientFd].getNickName() + " #" + channelName + " :End of /NAMES list \r\n";
			send(clientFd, replyEnd.c_str(), replyEnd.size(), 0);
		} else {
			// Channel not found
			string replyError = ":" + server->getServerName() + " 403 " + server->clients[clientFd].getNickName() + " " + channelName + " :No such channel \r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
		}
	}

	return true;
}
