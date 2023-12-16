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
	cout << "You are in INVITE execute" << endl;

	if (args.empty() || clientFd < 0) {
		std::cout << "Not enough param or client doesn't exist" << std::endl;
		return false;
	}

	std::string name;
	std::string targetUser;
	std::istringstream iss(args);

	// Find the client by nickname in the current channel
	Client *invitingClient = server->getClientByFd(clientFd);

	// Get the channel name and target user
	iss >> targetUser >> name;

	// Trim leading and trailing whitespaces from the channel name
	name = trim(name);

	// Remove '#' from the channel name
	std::string channelName = name.substr(1);

	// Get the channel object by name
	Channels *channel = server->getChannelByName(channelName);

	if (channel != nullptr) {
		// Check if the inviting client is a member of the channel
		if (!channel->hasUser(invitingClient)) {
			// Inviting client is not a member of the channel
			std::string replyError = ":" + server->getServerName() + " 442 " + invitingClient->getNickName() + " :You're not on that channel\r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			return false;
		}

		// Check if the channel has invite-only mode set
		if (channel->getInviteOnly() && !channel->isOperator(invitingClient)) {
			// Channel has invite-only mode set and inviting client is not a channel operator
			std::string replyError = ":" + server->getServerName() + " 482 " + invitingClient->getNickName() + " :You're not channel operator\r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			return false;
		}

		// Find the target user by nickname
		Client *targetClient = server->getClientByNickname(targetUser);

		if (targetClient != nullptr) {
			// Check if the target user is already on the channel
			if (channel->hasUser(targetClient)) {
				// Target user is already on the channel
				std::string replyError = ":" + server->getServerName() + " 443 " + invitingClient->getNickName() + " :is already on channel\r\n";
				send(clientFd, replyError.c_str(), replyError.size(), 0);
				return false;
			}

			// Invite the target user to the channel
			channel->addUsers(targetClient, 2);

			// Send RPL_INVITING numeric to the command issuer
			std::string replyInviting = ":" + server->getServerName() + " 341 " + invitingClient->getNickName() + " " + targetClient->getNickName() + " #" + channelName + " :Inviting " + targetClient->getNickName() + " to " + channelName + "\r\n";
			send(clientFd, replyInviting.c_str(), replyInviting.size(), 0);

			// Send INVITE message to the target user
			std::string inviteMessage = ":" + invitingClient->getNickName() + " INVITE " + targetClient->getNickName() + " :" + channelName + "\r\n";
			send(clientFd, inviteMessage.c_str(), inviteMessage.size(), 0);
		} else {
			// Target user not found
			std::string replyError = ":" + server->getServerName() + " 401 " + invitingClient->getNickName() + " " + targetUser + " :No such target user\r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			return false;
		}
	} else {
		// Channel not found
		std::string replyError = ":" + server->getServerName() + " 403 " + invitingClient->getNickName() + " " + channelName + " :No such channel\r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
		return false;
	}

	return true;
}
