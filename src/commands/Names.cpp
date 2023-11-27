#include "../../inc/commands/Names.hpp"

using std::cout;
using std::endl;

Names::Names() : command("NAMES") {}

Names::~Names() {}

Names::Names(Names const &src) {
	*this = src;
}

bool Names::execute(Server *server, std::string args, int clientFd) {
	cout << "You are in NAMES execute" << endl;
	if (args.empty() || clientFd < 0){
		cout << "Not enough param or client doesn't exists" << endl;
		return false;
	}

	//Get the channel size
	int size = server->channel.size();
	cout << "Nb of channel is: " << size << endl;

	for (int i = 0; i < size; ++i) {
		Channels* channel = &(server->channel[i]); // Access the i-th channel
		cout << "Channel #" << i << " name is: " << channel->getChannelName() << endl;

	}

	return true;
}