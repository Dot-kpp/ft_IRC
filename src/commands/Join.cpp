#include "../../inc/commands/Join.hpp"

using std::cout;
using std::endl;

Join::Join() : command("JOIN") {}

Join::~Join() {}

Join::Join(Join const &src) {
	*this = src;
}

bool compareChannelName(const Channels& obj, const std::string& channelName) {
    return obj.getChannelName() == channelName;
}


bool Join::execute(Server *server, std::string args, int clientFd) {
    cout << "You are in JOIN execute" << endl;

    if (args.empty() || clientFd < 0){
        cout << "Not enough param or client doesn't exists" << endl;
        return false;
    }

    // Get the Client object associated with clientFd
    Client* client = server->getClientByFd(clientFd);
    if (!client) {
        cout << "Client with fd " << clientFd << " does not exist" << endl;
        return false;
    }
	cout << "	args = " << args << endl;
    std::string name;
    std::istringstream iss(args);

    // Get the channel name
    std::getline(iss, name, ' ');
	std::string channelName = name.substr(1);

    // Get the channel object by name
    Channels *channel = server->getChannelByName(name);

    if (channel == nullptr) {
        // Channel does not exist, create it
        Channels *newChannel = new Channels; // Assuming Channels is a class
        cout << "Channel " << name << " does not exist, creating it..." << endl;
		newChannel->addUsers(client, 1); // Pass the Client object and role ID
        newChannel->setChannelName(channelName); // Set the name of the channel
		cout << "Channel name: " << channelName << endl;
//        server->channel.push_back(newChannel);
        cout << "Channel " << channelName << " created and client " << clientFd << " added to it." << endl;
    } else {
        // Channel exists, add the client to it
        channel->addUsers(client, 2); // Pass the Client object and role ID
        cout << "Client " << clientFd << " added to the existing channel " << name << endl;
    }

    return true;
}