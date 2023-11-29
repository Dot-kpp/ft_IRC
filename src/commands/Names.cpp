#include "../../inc/commands/Names.hpp"

using std::cout;
using std::endl;

Names::Names() : command("NAMES") {}

Names::~Names() {}

Names::Names(Names const &src) {
	*this = src;
}

//bool Names::execute(Server *server, std::string args, int clientFd) {
//	cout << "You are in NAMES execute" << endl;
//	if (args.empty() || clientFd < 0){
//		cout << "Not enough param or client doesn't exists" << endl;
//		return false;
//	}
//
//	//Get the channel size
//	int size = server->channel.size();
//	cout << "Nb of channel is: " << size << endl;
//
//	for (int i = 0; i < size; ++i) {
//		Channels* channel = &(server->channel[i]); // Access the i-th channel
//		cout << "Channel #" << i << " name is: " << channel->getChannelName() << endl;
//		cout << "users in Channel #" << i << ": " << channel->getClients() << endl << endl;
//	}
//
//	return true;
//}

//bool Names::execute(Server *server, std::string args, int clientFd) {
//	cout << "You are in NAMES execute" << endl;
//	cout << "Args: " << args << endl;
//	if (args.empty() || clientFd < 0){
//		cout << "Not enough param or client doesn't exist" << endl;
//		return false;
//	}
//
//	// Get the channel size
//	int size = server->channel.size();
//	cout << "Nb of channels is: " << size << endl;
//
//	for (int i = 0; i < size; ++i) {
//		Channels* channel = &(server->channel[i]); // Access the i-th channel
//		cout << "Channel #" << i << " name is: " << channel->getChannelName() << endl;
//		cout << "Users in Channel #" << i << ": " << channel->getClients() << endl;
//	}
//
//	cout << endl; // Print a newline to separate different executions of the NAMES command
//
//	return true;
//}

bool Names::execute(Server *server, std::string args, int clientFd) {
	cout << "You are in NAMES execute" << endl;
	if (args.empty() || clientFd < 0){
		cout << "Not enough param or client doesn't exist" << endl;
		return false;
	}

	// Parse the channel name from the args parameter
	std::string channelName;
	std::istringstream iss(args);
	iss >> channelName;

	// Find the channel by name
	Channels* channel = server->getChannelByName(channelName);

	// Check if the channel exists
	if (channel == NULL) {
		cout << "Channel '" << channelName << "' not found" << endl;
		return false;
	}

	cout << "Users in Channel '" << channelName << "': ";

	// Print users in the specified channel
	std::string users = channel->getClients();
	std::cout << users << std::endl;

	return true;
}


