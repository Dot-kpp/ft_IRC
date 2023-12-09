#include "../../inc/commands/Join.hpp"

using std::cout;
using std::endl;

Join::Join() : command("JOIN") {}

Join::~Join() {}

Join::Join(Join const &src) {
	*this = src;
}

bool compareChannelName(const Channels& obj, const std::string& channelName) {
    return obj.getChannelName() == channelName;
}

bool Join::execute(Server *server, std::string args, int clientFd) {
	cout << "You are in JOIN execute" << endl;

	if (args.empty() || clientFd < 0){
		std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " JOIN " + " :No such channel \r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
		return false;
	}

	// Get the Client object associated with clientFd
	Client* client = server->getClientByFd(clientFd);
	if (!client) {
		cout << "Client with fd " << clientFd << " does not exist" << endl;
		return false;
	}

	std::string name;
	std::istringstream iss(args);

	// Get the channel name
	std::getline(iss, name, ' ');

	// Get the channel object by name
	std::string channelName = name.substr(1);
	Channels *channel = server->getChannelByName(channelName);

	if (channel == nullptr) {
		// Channel does not exist, create it
		Channels newChannel; // Assuming Channels is a class
		newChannel.addUsers(client, 1); // Pass the Client object and role ID
		newChannel.setChannelName(channelName); // Set the name of the channel
		server->channel.push_back(newChannel); // Add the channel to the server's channel list
		cout << "Channel " << channelName << " created and client " << clientFd << " added to it." << endl;
	} else {
		if (channel->hasUser(client)) {
			// User is already in the channel
			std::string replyError = ":" + server->getServerName() + " 443 " + server->clients[clientFd].getNickName() + " #" + channelName + " :is already on channel\r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			return false;
		}
		if(channel->getHasKey()) {
			// Channel has a key, check if the user provided the correct key
			std::string providedKey;
			iss >> providedKey;

			if (providedKey != channel->getKey()) {
				// Incorrect key provided
				std::string replyError = ":" + server->getServerName() + " 475 " + server->clients[clientFd].getNickName() + " #" + channelName + " :Cannot join channel (+k) - bad key \r\n";
				send(clientFd, replyError.c_str(), replyError.size(), 0);
				return false;
			}
		}
		if(channel->getUserLimitValue() == channel->getUsers().size() && channel->getHasLimit()) {
			// Channel is full
			std::string replyError = ":" + server->getServerName() + " 471 " + server->clients[clientFd].getNickName() + " #" + channelName + " :Cannot join channel (+l) - channel is full \r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			return false;
		}
		// Channel exists, add the client to it
		channel->addUsers(client, 2); // Pass the Client object and role ID
		cout << "Client " << clientFd << " added to the existing channel " << name << endl;
	}

	return true;
}
