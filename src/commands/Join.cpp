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
	if (args.empty() || clientFd < 0) {
		std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " JOIN :Not enough parameters \r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
		return false;
	}

	// Get the Client object associated with clientFd
	Client *client = server->getClientByFd(clientFd);
	if (!client) {
		std::cout << "Client with fd " << clientFd << " does not exist" << std::endl;
		return false;
	}

	// Get the channel name
	std::istringstream iss(args);
	std::string channelInfo;

	while (std::getline(iss, channelInfo, ',')) {
		size_t spacePos = channelInfo.find(' ');

		std::string channelName;
		std::string providedKey;

		if (spacePos != std::string::npos) {
			// Split channelInfo into channelName and providedKey
			channelName = channelInfo.substr(0, spacePos);
			providedKey = channelInfo.substr(spacePos + 1);

			providedKey.erase(std::remove(providedKey.begin(), providedKey.end(), '\n'), providedKey.end());
			providedKey.erase(std::remove(providedKey.begin(), providedKey.end(), '\r'), providedKey.end());

		} else {
			// No space found, use channelInfo as channelName
			channelName = channelInfo;
			providedKey.clear();
		}

		// Remove \n and \r characters from channelName
		channelName.erase(std::remove(channelName.begin(), channelName.end(), '\n'), channelName.end());
		channelName.erase(std::remove(channelName.begin(), channelName.end(), '\r'), channelName.end());
		if (channelName.front() != '#' || channelName.find('#', 1) != std::string::npos) {
			std::string replyError = ":" + server->getServerName() + " 476 " + channelName + " :Bad Channel Mask\r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			return false;
		}

		if(channelName.size() > 12) {
			std::string replyError = ":" + server->getServerName() + " 400 " + channelName + " :Bad Channel length (>15 characters)\r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			return false;
		}

		Channels *channel = server->getChannelByName(channelName);

		if (channel == nullptr) {
			// Channel does not exist, create it
			Channels newChannel;
			newChannel.addUsers(client, 1);
			newChannel.setChannelName(channelName);
			server->channel.push_back(newChannel);

			std::string reply = ":" + server->clients[clientFd].getNickName() + " JOIN " + channelName + "\r\n";
			send(clientFd, reply.c_str(), reply.size(), 0);

			std::string operatorMessage = ":" + server->getServerName() + " 381 " + server->clients[clientFd].getNickName() + " :You are now an IRC operator \r\n";
			send(clientFd, operatorMessage.c_str(), operatorMessage.size(), 0);
			// Send the MODE numeric reply indicating channel modes
			std::string replyMode = ":" + server->getServerName() + " 324 " + client->getNickName() + " " + channelName + " +o " + client->getNickName() + "\r\n";
			send(clientFd, replyMode.c_str(), replyMode.size(), 0);

			std::cout << "Channel " << channelName << " created and client " << server->clients[clientFd].getNickName() << " added to it." << std::endl;
		} else {
			if (channel->getInviteOnly() && !channel->isUserInvited(client)) {
				// Channel is invite-only and user is not invited
				std::string replyError = ":" + server->getServerName() + " 473 " + server->clients[clientFd].getNickName() + " " + channelName + " :Cannot join channel (+i) - not invited \r\n";
				send(clientFd, replyError.c_str(), replyError.size(), 0);
				return false;
			}
			if (channel->hasUser(client)) {
				// User is already in the channel
				std::string replyError = ":" + server->getServerName() + " 443 " + server->clients[clientFd].getNickName() + " " + channelName + " :is already on channel\r\n";
				send(clientFd, replyError.c_str(), replyError.size(), 0);
				return false;
			}
			if (channel->getUserLimitValue() == channel->getUsers().size() && channel->getHasLimit()) {
				std::string replyError = ":" + server->getServerName() + " 471 " + server->clients[clientFd].getNickName() + " " + channelName + " :Cannot join channel (+l) - channel is full \r\n";
				send(clientFd, replyError.c_str(), replyError.size(), 0);
				return false;
			}
			if (channel->getHasKey()) {
				std::cout << "providedKey: [" << providedKey << "]" << std::endl;
				std::cout << "channel->getKey(): [" << channel->getKey() << "]" << std::endl;
				if (providedKey != channel->getKey()) {
					std::string replyError =
							":" + server->getServerName() + " 475 " + server->clients[clientFd].getNickName() + " " + channelName + " :Cannot join channel (+k) - bad key \r\n";
					send(clientFd, replyError.c_str(), replyError.size(), 0);
					return false;
				}
			}

			// Channel exists, add the client to it
			channel->addUsers(client, 2);

			// Send the JOIN message to the other clients in the channel
			std::string reply = ":" + server->clients[clientFd].getNickName() + " JOIN " + channelName + "\r\n";
			send(clientFd, reply.c_str(), reply.size(), 0);
			server->broadcastToChannel(channel->getChannelName(), reply, clientFd, client->getNickName());
			std::cout << "Client " << clientFd << " added to the existing channel " << channelName << std::endl;

			// send the user list to the client
			std::string userList = ":" + server->getServerName() + " 353 " + server->clients[clientFd].getNickName() + " = " + channelName + " :";
			for (std::map<Client *, int>::const_iterator it = channel->getUsers().begin(); it != channel->getUsers().end(); ++it) {
				userList += it->first->getNickName() + " ";
			}
			userList += "\r\n";
			send(clientFd, userList.c_str(), userList.size(), 0);
			std::string endOfUserList = ":" + server->getServerName() + " 366 " + server->clients[clientFd].getNickName() + " " + channelName + " :End of /NAMES list\r\n";
			send(clientFd, endOfUserList.c_str(), endOfUserList.size(), 0);

			// send the topic to the client
			std::string topicMessage = ":" + server->getServerName() + " 332 " + server->clients[clientFd].getNickName() + " " + channelName + " :" + channel->getTopic() + "\r\n";
			send(clientFd, topicMessage.c_str(), topicMessage.size(), 0);
		}
	}
	return true;
}
