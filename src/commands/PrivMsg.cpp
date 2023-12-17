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

	if (args.empty() || clientFd < 0) {
		std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " PRIVMSG :Not enough parameters \r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
		return false;
	}

	std::istringstream iss(args);
	std::string target;
	std::string message;

	iss >> target;
	Client *client = server->getClientByFd(clientFd);

	std::getline(iss, message);
	if(target.find('#') != std::string::npos) {
		Channels *channel = server->getChannelByName(target);

		if (channel == nullptr) {
			std::string replyError = ":" + server->getServerName() + " 401 " + client->getNickName() + " " + target + " :No such nick/channel \r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			std::cout << "Channel '" << target << "' not found" << std::endl;
			return false;
		}
		message = message.substr(2);

		std::string reply = ":" + client->getNickName() + " PRIVMSG " + channel->getChannelName() + " :" + message + "\r\n";
		server->broadcastToChannel(channel->getChannelName(), reply, clientFd, client->getNickName());

	} else {
		Client *targetClient = server->getClientByNickname(target);
		if (targetClient == nullptr) {
			std::string replyError = ":" + server->getServerName() + " 401 " + client->getNickName() + " " + target + " :No such nick/channel \r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			std::cout << "Client '" << target << "' not found" << std::endl;
			return false;
		}

		message = message.substr(2);
		server->sendMessageToClient(targetClient->getClientFd(), message, targetClient->getNickName(), client->getNickName());
		std::cout << "Message sent to client " << targetClient->getClientFd() << std::endl;
	}

	return true;
}
