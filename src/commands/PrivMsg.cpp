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
	cout << "You are in PRIVMSG execute" << endl;
	cout << "	args: " << args << endl;

	// Find the target (if there is a '#', it's a channel, otherwise it's a user)
	std::istringstream iss(args);
	std::string target;
	std::string message;

	iss >> target;
	cout << "target: " << target << endl;
	Client *client = server->getClientByFd(clientFd);

	// Use std::getline to get the rest of the string
	std::getline(iss, message);

	if(target.find('#') != std::string::npos) {
		cout << "This is a channel" << endl;

		// Find the channel by name
		Channels *channel = server->getChannelByName(target);

		if (channel == nullptr) {
			std::cout << "Channel '" << target << "' not found" << std::endl;
			return false;
		}
		message = message.substr(2);

		std::string reply = ":" + client->getNickName() + " PRIVMSG " + channel->getChannelName() + " :" + message + "\r\n";
		server->broadcastToChannel(channel->getChannelName(), reply, clientFd, client->getNickName());

	} else {
		Client *targetClient = server->getClientByNickname(target);
		if (targetClient == nullptr) {
			std::cout << "Client '" << target << "' not found" << std::endl;
			return false;
		}

		message = message.substr(2);
		server->sendMessageToClient(targetClient->getClientFd(), message, targetClient->getNickName(), client->getNickName());
		std::cout << "Message sent to client " << targetClient->getClientFd() << std::endl;
	}

	return true;
}
