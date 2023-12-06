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
        std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " JOIN " + " :No such channel \r\n";
        send(clientFd, replyError.c_str(), replyError.size(), 0);
        return false;
    }

    // Get the Client object associated with clientFd
    Client* client = server->getClientByFd(clientFd);
    if (!client) {
        cout << "Client with fd " << clientFd << " does not exist" << endl;
        return false;
    }

    std::string name;
    std::istringstream iss(args);

    // Get the channel name
    std::getline(iss, name, ' ');

    // Get the channel object by name
    Channels *channel = server->getChannelByName(name);

    if (channel == nullptr) {
        // Channel does not exist, create it
        Channels newChannel; // Assuming Channels is a class
        newChannel.addUsers(client, 2); // Pass the Client object and role ID
        std::string channelName = name.substr(1);
        newChannel.setChannelName(channelName); // Set the name of the channel
        server->channel.push_back(newChannel);
        cout << "Channel " << channelName << " created and client " << clientFd << " added to it." << endl;
    } else {
        // Channel exists, add the client to it
        channel->addUsers(client, 2); // Pass the Client object and role ID
        cout << "Client " << clientFd << " added to the existing channel " << name << endl;
    }

    return true;
}