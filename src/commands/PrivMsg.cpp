#include "../../inc/commands/PrivMsg.hpp"

using std::cout;
using std::endl;
using std::string;

PrivMsg::PrivMsg() : command("PRIVMSG") { }

PrivMsg::PrivMsg(PrivMsg const &src) {
	*this = src;
}

PrivMsg::~PrivMsg() { }

bool PrivMsg::execute(Server *server, std::string args, int clientFd) {
	(void)server;
	(void)clientFd;
	cout << "You are in PRIVMSG execute" << endl;

	// Find the target (if there is a '#', it's a channel, otherwise it's a user)
	std::istringstream iss(args);
	std::string target;
	std::string message;

	iss >> target;
	cout << "target: " << target << endl;

	// Use std::getline to get the rest of the string
	std::getline(iss, message);

	if(target.find('#') != std::string::npos) {
		cout << "This is a channel" << endl;

		// Find the channel by name
		std::string channelName = target.substr(1);
		Channels *channel = server->getChannelByName(channelName);

		if (channel == nullptr) {
			std::cout << "Channel '" << channelName << "' not found" << std::endl;
			return false;
		}
		cout << "msg_to_chan: " << message << endl;

	} else {
		cout << "This is a user" << endl;
		// Find the client by nickname in the current channel
		Client *targetClient = server->getClientByNickname(target);
		if (targetClient == nullptr) {
			std::cout << "Client '" << target << "' not found" << std::endl;
			return false;
		}
		cout << "msg_to_user: " << message << endl;

	}

	return true;
}

//		// Check if the channel exists
//		if (channel == nullptr) {
//			std::cout << "Channel '" << target << "' not found" << std::endl;
//			return false;
//		}
//
//		// Find the client by nickname in the current channel
//		Client *targetClient = server->getClientByFd(clientFd);
//
//		// Remove '#' from the channel name
//		std::string channelName = target.substr(1);
//
//		// Send the message to all clients in the channel
//		for (std::vector<Client *>::iterator it = channel->getClients().begin(); it != channel->getClients().end(); ++it) {
//			Client *client = *it;
//
//			// Don't send the message to the client who sent it
//			if (client->getNickname() == targetClient->getNickname()) {
//				continue;
//			}
//
//			// Send the message to the client
//			client->sendMessage(targetClient->getNickname(), message);
//		}
//	}
//	else
//	{
//		// Find the client by nickname in the current channel
//		Client *targetClient = server->getClientByFd(clientFd);
//
//		// Send the message to the client
//		targetClient->sendMessage(targetClient->getNickname(), message);
//	}

//	std::getline(iss, message);
//
//	if(target.empty() || message.empty()) {
//		std::cout << "Not enough parameters for PRIVMSG command" << std::endl;
//		return false;
//	}
