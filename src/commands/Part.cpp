#include "../../inc/commands/Part.hpp"

Part::Part() : command("PART") {}

Part::~Part() {}

Part::Part(Part const &src) {
	*this = src;
}

bool Part::execute(Server *server, std::string args, int clientFd) {
    std::cout << "You are in PART execute" << std::endl;

    if (args.empty() || clientFd < 0){
        std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " PART" + " :No such channel \r\n";
        send(clientFd, replyError.c_str(), replyError.size(), 0);
        return false;
    }

    // Get the Client object associated with clientFd
    Client* client = server->getClientByFd(clientFd);
    if (!client) {
        std::cout << "Client with fd " << clientFd << " does not exist" << std::endl;
        return false;
    }

	std::string name;

    // iss >> name;
	// Get the channel name
    size_t hashPos = args.find('#');
    size_t spacePos = args.find(' ', hashPos); // Find the space after '#'
    if (hashPos != std::string::npos && hashPos + 1 < args.size()) {
        // Calculate the length of the channel name
        size_t nameLength = (spacePos != std::string::npos) ? spacePos - hashPos - 1 : std::string::npos;
        // Extract the channel name
        name = args.substr(hashPos + 1, nameLength);
	} else {
		// '#' not found or it's the last character in args
		std::cout << "Invalid command format. Expected '#' followed by channel name." << std::endl;
		return false;
	}
	// Get the channel object by name


	std::string channelName = "#" + name;
    Channels *channel = server->getChannelByName(channelName);
    if (channel == nullptr) {
        // Channel does not exist
        std::string replyError = ":" + server->getServerName() + " 403 " + server->clients[clientFd].getNickName() + " " + channelName + " :No such channel\r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
        std::cout << "Channel " << channelName << " does not exist" << std::endl;
        return false;
    }  // This closing brace was missing

    std::cout << "here" << std::endl;
    if (!channel->hasUser(client)) {
        std::string replyError = server->clients[clientFd].getNickName() + channelName + ":" + "You're not on that channel\r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
        std::cout << "Client " << clientFd << " is not a member of the channel " << name << std::endl;
        return false;
    } else {
        // Client is a member of the channel, remove them
        channel->removeUser(client);
        std::cout << "Client " << clientFd << " removed from channel " << name << std::endl;
        std::string reply = ":" + server->clients[clientFd].getNickName() + " PART " + channelName + "\r\n";
        send(clientFd, reply.c_str(), reply.size(), 0);
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