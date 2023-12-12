#include "../../inc/commands/Join.hpp"

Join::Join() : command("JOIN") {}

Join::~Join() {}

Join::Join(Join const &src) {
	*this = src;
}

bool compareChannelName(const Channels& obj, const std::string& channelName) {
    return obj.getChannelName() == channelName;
}

bool Join::execute(Server *server, std::string args, int clientFd) {
	std::cout << "You are in JOIN execute" << std::endl;

	if (args.empty() || clientFd < 0){
		std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " JOIN " + " :No such channel \r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
		return false;
	}

	// Get the Client object associated with clientFd
	Client* client = server->getClientByFd(clientFd);
	if (!client) {
		std::cout << "Client with fd " << clientFd << " does not exist" << std::endl;
		return false;
	}

	// Get the channel name
	std::string name;
	std::istringstream iss(args);
	
	iss >> name;

	// Find the position of the first '#'
	// size_t hashPos = args.find('#');
	// if (hashPos != std::string::npos && hashPos + 1 < args.size()) {
	// 	// Extract the channel name
	// 	name = args.substr(hashPos + 1);
	// } else {
	// 	// '#' not found or it's the last character in args
	// 	std::cout << "Invalid command format. Expected '#' followed by channel name." << std::endl;
	// 	return false;
	// }

	// Get the channel object by name
	std::string channelName = name;
	// if (!name.empty()) {
	// 	channelName = name;
	// }
	Channels *channel = server->getChannelByName(channelName);
	std::cout << "Channel name: " << channelName.size() << std::endl;
	if (channel == nullptr) {
		// Channel does not exist, create it
		Channels newChannel; // Assuming Channels is a class
		newChannel.addUsers(client, 1); // Pass the Client object and role ID
		newChannel.setChannelName(channelName); // Set the name of the channel
		server->channel.push_back(newChannel); // Add the channel to the server's channel list
		std::string reply = ":" + server->clients[clientFd].getNickName() + " JOIN " + channelName + "\r\n";
		send(clientFd, reply.c_str(), reply.size(), 0);
		std::cout << "Channel " << channelName << " created and client " << server->clients[clientFd].getNickName() << " added to it." << std::endl;
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
		if(channel->getInviteOnly() && !channel->hasUser(client)) {
			// Channel is invite-only and user is not invited
			std::string replyError = ":" + server->getServerName() + " 473 " + server->clients[clientFd].getNickName() + " #" + channelName + " :Cannot join channel (+i) - not invited \r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			return false;
		}
		// Channel exists, add the client to it
		channel->addUsers(client, 2); // Pass the Client object and role ID
		std::cout << "Client " << clientFd << " added to the existing channel " << name << std::endl;
	}

	return true;
}
