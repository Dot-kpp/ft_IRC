#include "../../inc/commands/Invite.hpp"

using std::cout;
using std::endl;
using std::string;

Invite::Invite() : command("INVITE") {}

Invite::~Invite() {}

Invite::Invite(Invite const &src) {
	*this = src;
}

std::string Invite::trim(const std::string& str) {
	size_t start = str.find_first_not_of(" \t\r\n");
	size_t end = str.find_last_not_of(" \t\r\n");

	if (start == std::string::npos || end == std::string::npos)
		return "";

	return str.substr(start, end - start + 1);
}

bool Invite::execute(Server *server, std::string args, int clientFd) {

	if (args.empty() || clientFd < 0) {
		std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " INVITE :Not enough parameters \r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
		return false;
	}

	std::string channelName;
	std::string targetUser;
	std::istringstream iss(args);

	// Get the channel name and target user
	iss >> targetUser >> channelName;


	Client *client = server->getClientByFd(clientFd);

	// Get the channel object by name
	Channels *channel = server->getChannelByName(channelName);

	if (channel != nullptr) {
		// Check if the user is an operator of the channel
		if (!channel->isOperator(client)) {
			std::string replyError = ":" + server->getServerName() + " 482 " + client->getNickName() + " :You're not channel operator\r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			return false;
		}

		Client *targetClient = server->getClientByNickname(targetUser);

		if (targetClient != nullptr) {
			// Check if the target user is already on the channel
			if (channel->hasUser(targetClient)) {
				// Target user is already on the channel
				std::string replyError = ":" + server->getServerName() + " 443 " + targetClient->getNickName() + " :is already on channel\r\n";
				send(clientFd, replyError.c_str(), replyError.size(), 0);
				return false;
			}

			// Add target user to the channel invite list
			channel->addInvitedUser(targetClient);

			// Send RPL_INVITING numeric to the command issuer
			std::string replyInviting = ":" + server->getServerName() + " 341 " + client->getNickName() + " " + targetClient->getNickName() + " " + channelName + " :Inviting " + targetClient->getNickName() + " to " + channelName + "\r\n";
			send(clientFd, replyInviting.c_str(), replyInviting.size(), 0);

			// Send INVITE message to the target user
			std::string inviteMessage = ":" + client->getNickName() + " INVITE " + targetClient->getNickName() + " :" + channelName + "\r\n";
			send(targetClient->getClientFd(), inviteMessage.c_str(), inviteMessage.size(), 0);
		} else {
			// Target user not found
			std::string replyError = ":" + server->getServerName() + " 401 " + client->getNickName() + " " + targetUser + " :No such target user\r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			return false;
		}
	} else {
		// Channel not found
		std::string replyError = ":" + server->getServerName() + " 403 " + client->getNickName() + " " + channelName + " :No such channel\r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
		return false;
	}

	return true;
}
