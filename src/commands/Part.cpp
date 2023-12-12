#include "../../inc/commands/Part.hpp"

Part::Part() : command("PART") {}

Part::~Part() {}

Part::Part(Part const &src) {
	*this = src;
}

bool Part::execute(Server *server, std::string args, int clientFd) {
    std::cout << "You are in PART execute" << std::endl;

    if (args.empty() || clientFd < 0){
        std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " PART " + " :No such channel \r\n";
        send(clientFd, replyError.c_str(), replyError.size(), 0);
        return false;
    }

    // Get the Client object associated with clientFd
    Client* client = server->getClientByFd(clientFd);
    if (!client) {
        std::cout << "Client with fd " << clientFd << " does not exist" << std::endl;
        return false;
    }

    std::string command, name;
    std::istringstream iss(args);

	// Get the channel name
	size_t pos = args.find(' ');
	if (pos != std::string::npos) {
		// Extract the command
		command = args.substr(0, pos);

		// Find the position of the first '#'
		size_t hashPos = args.find('#', pos);
		if (hashPos != std::string::npos && hashPos + 1 < args.size()) {
			// Extract the channel name
			name = args.substr(hashPos + 1);
		}
	}

	// Get the channel object by name
	std::string channelName;
	if (!name.empty()) {
		channelName = name.substr(1);
	}
	Channels *channel = server->getChannelByName(channelName);

    if (channel == nullptr) {
        // Channel does not exist
        std::cout << "Channel " << name << " does not exist" << std::endl;
        return false;
    }  // This closing brace was missing

    if (!channel->hasUser(client)) {
        std::cout << "Client " << clientFd << " is not a member of the channel " << name << std::endl;
        return false;
    } else {
        // Client is a member of the channel, remove them
        channel->removeUser(client);
        std::cout << "Client " << clientFd << " removed from the existing channel " << name << std::endl;

        // Send a message to the other members of the channel
        std::stringstream ss;
        ss << "Client " << clientFd << " has left the channel.";
        std::string message = ss.str();
        std::map<Client *, int>::const_iterator it;
        for (it = channel->getUsers().begin(); it != channel->getUsers().end(); ++it) {
            if (it->first != client) {
                it->first->sendMessage(message);
            }
        }
    }
    return true;
}