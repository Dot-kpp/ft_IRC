#include "../../inc/commands/Invite.hpp"

using std::cout;
using std::endl;
using std::string;

Invite::Invite() : command("INVITE") {}

Invite::~Invite() {}

Invite::Invite(Invite const &src) {
	*this = src;
}

bool Invite::execute(Server *server, std::string args, int clientFd) {
	(void)server;
	(void)args;
	(void)clientFd;

	cout << "You are in INVITE execute" << endl;
//	Channels *channel = server->getChannelByName(channelName);

//	const std::vector<Client *> &clientsList = channel->getClients();
	return true;
}
