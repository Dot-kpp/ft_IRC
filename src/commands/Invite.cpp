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

	std::string channelName;
	std::string targetUser;
	std::istringstream iss(args);

	// Get the channel name and target user
	iss >> targetUser >> channelName;


	Client *client = server->getClientByFd(clientFd);

	// Get the channel object by name
	Channels *channel = server->getChannelByName(channelName);

	if (channel != nullptr) {
		Client *targetClient = server->getClientByNickname(targetUser);

//		// Check if the inviting client is a member of the channel
//		if (!channel->hasUser(invitingClient)) {
//			// Inviting client is not a member of the channel
//			std::string replyError = ":" + server->getServerName() + " 442 " + invitingClient->getNickName() + " :You're not on that channel\r\n";
//			send(clientFd, replyError.c_str(), replyError.size(), 0);
//			return false;
//		}


//		// Check if the channel has invite-only mode set
//		if (channel->getInviteOnly() && !channel->isOperator(invitingClient)) {
//			// Channel has invite-only mode set and inviting client is not a channel operator
//			std::string replyError = ":" + server->getServerName() + " 482 " + invitingClient->getNickName() + " :You're not channel operator\r\n";
//			send(clientFd, replyError.c_str(), replyError.size(), 0);
//			return false;
//		}


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

//			// Invite the target user to the channel
//			channel->addUsers(targetClient, 2);

			// Send RPL_INVITING numeric to the command issuer
			std::string replyInviting = ":" + server->getServerName() + " 341 " + client->getNickName() + " " + targetClient->getNickName() + " #" + channelName + " :Inviting " + targetClient->getNickName() + " to " + channelName + "\r\n";
			send(clientFd, replyInviting.c_str(), replyInviting.size(), 0);

			// Send INVITE message to the target user
			std::string inviteMessage = ":" + client->getNickName() + " INVITE " + targetClient->getNickName() + " :" + channelName + "\r\n";
			send(clientFd, inviteMessage.c_str(), inviteMessage.size(), 0);
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

//bool Invite::execute(Server *server, std::string args, int clientFd) {
//	cout << "You are in INVITE execute" << endl;
//	std::cout << "	args: " << args << std::endl;
//
//	if (args.empty() || clientFd < 0) {
//		std::cout << "Not enough param or client doesn't exist" << std::endl;
//		return false;
//	}
//
//	std::string channelName;
//	std::string targetUser;
//	std::istringstream iss(args);
//
//	// Get the channel name and target user
//	iss >> targetUser >> channelName;
//
//	// Find the client by nickname in the current channel
//	Client *client = server->getClientByFd(clientFd);
//	if (!client) {
//		std::cout << "Client with fd " << clientFd << " does not exist" << std::endl;
//		return false;
//	}
//
//	std::cout << "targetUser: " << targetUser << std::endl;
//	// Find the target user by nickname
//	Client *targetClient = server->getClientByNickname(targetUser);
//	if (!targetClient) {
//		std::string replyError = ":" + server->getServerName() + " 401 " + targetClient->getNickName() + " " + targetUser + " :No such target user\r\n";
//		send(clientFd, replyError.c_str(), replyError.size(), 0);
//		std::cout << "Target user not found" << std::endl;
//		return false;
//	}
//
//	// Get the channel object by name
//	Channels *channel = server->getChannelByName(channelName);
//	if (!channel) {
//		std::cout << "Channel not found" << std::endl;
//		return false;
//	}
//
//	if (channel != nullptr) {
////		Check if the inviting client is a member of the channel
//		if (!channel->hasUser(targetClient)) {
//			// Inviting client is not a member of the channel
//			std::string replyError = ":" + server->getServerName() + " 442 " + targetClient->getNickName() + " :You're not on that channel\r\n";
//			send(clientFd, replyError.c_str(), replyError.size(), 0);
//			return false;
//		}
//
//		// Check if the channel has invite-only mode set
//		if (channel->getInviteOnly() && !channel->isOperator(targetClient)) {
//			// Channel has invite-only mode set and inviting client is not a channel operator
//			std::string replyError = ":" + server->getServerName() + " 482 " + targetClient->getNickName() + " :You're not channel operator\r\n";
//			send(clientFd, replyError.c_str(), replyError.size(), 0);
//			return false;
//		}
//
//
//		if (targetClient != nullptr) {
//			// Check if the target user is already on the channel
//			if (channel->hasUser(targetClient)) {
//				// Target user is already on the channel
//				std::string replyError = ":" + server->getServerName() + " 443 " + targetClient->getNickName() + " :is already on channel\r\n";
//				send(clientFd, replyError.c_str(), replyError.size(), 0);
//				return false;
//			}
//
//			// Invite the target user to the channel
//			channel->addUsers(targetClient, 2);
//
//			// Send INVITE message to the target user
//			std::string inviteMessage = ":" + client->getNickName() + " INVITE " + targetClient->getNickName() + " :" + channelName + "\r\n";
//			send(targetClient->getClientFd(), inviteMessage.c_str(), inviteMessage.size(), 0);
//
//			// Send RPL_INVITING numeric to the command issuer
//			std::string replyInviting = ":" + server->getServerName() + " 341 " + client->getNickName() + " " + targetClient->getNickName() + " " + channelName + "\r\n";
//			send(clientFd, replyInviting.c_str(), replyInviting.size(), 0);
//
//		} else {
//			// Target user not found
//			std::string replyError = ":" + server->getServerName() + " 401 " + targetClient->getNickName() + " " + targetUser + " :No such target user\r\n";
//			send(clientFd, replyError.c_str(), replyError.size(), 0);
//			return false;
//		}
//	} else {
//		// Channel not found
//		std::string replyError = ":" + server->getServerName() + " 403 " + client->getNickName() + " " + channelName + " :No such channel\r\n";
//		send(clientFd, replyError.c_str(), replyError.size(), 0);
//		return false;
//	}
//
//	return true;
//}