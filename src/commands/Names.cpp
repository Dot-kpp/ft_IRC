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

bool Names::execute(Server *server, std::string args, int clientFd)
{
	// Need to remove the below
	std::cout << "You are in NAMES execute" << std::endl;

	// Need to modify the below to send the appropriate replies
	if (args.empty() || clientFd < 0)
	{
		std::cout << "Not enough param or client doesn't exist" << std::endl;
		return false;
	}

	std::string channelName;
	std::istringstream iss(args);

	// Iterate through channel names separated by a comma
	while (getline(iss, channelName, ','))
	{
		// Trim leading and trailing whitespaces from the channel name
		channelName = trim(channelName);

		// Get the channel object by name
		Channels *channel = server->getChannelByName(channelName);

		if (channel != nullptr)
		{
			std::string namesList;

			// Start of NAMES list numeric reply
			std::string replyStart = ":" + server->getServerName() + " 353 " + server->clients[clientFd].getNickName() + " = " + channelName + " : ";
			send(clientFd, replyStart.c_str(), replyStart.size(), 0);

			// Iterate through clients in the channel and send their names
			const std::map<Client *, int> &usersMap = channel->getUsers();
			for (std::map<Client *, int>::const_iterator it = usersMap.begin(); it != usersMap.end(); ++it)
			{
				// Add the appropriate channel membership symbol here, e.g., "=" for public channel
				namesList += it->first->getNickName() + " ";
			}
			namesList += "\r\n";
			// Send the NAMES list
			send(clientFd, namesList.c_str(), namesList.size(), 0);

			// End of NAMES list numeric reply
			std::string replyEnd = ":" + server->getServerName() + " 366 " + server->clients[clientFd].getNickName() + " " + channelName + " :End of /NAMES list \r\n";
			send(clientFd, replyEnd.c_str(), replyEnd.size(), 0);
		}
		else
		{
			// Channel not found
			std::string replyError = ":" + server->getServerName() + " 403 " + server->clients[clientFd].getNickName() + " " + channelName + " :No such channel \r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
		}
	}

	return true;
}

