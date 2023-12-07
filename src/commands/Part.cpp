#include "../../inc/commands/Part.hpp"

using std::cout;
using std::endl;

Part::Part() : command("PART") {}

Part::~Part() {}

Part::Part(Part const &src) {
	*this = src;
}

bool Part::execute(Server *server, std::string args, int clientFd) {
    cout << "You are in PART execute" << endl;

    if (args.empty() || clientFd < 0){
        std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " PART " + " :No such channel \r\n";
        send(clientFd, replyError.c_str(), replyError.size(), 0);
        return false;
    }

    // Get the Client object associated with clientFd
    Client* client = server->getClientByFd(clientFd);
    if (!client) {
        cout << "Client with fd " << clientFd << " does not exist" << endl;
        return false;
    }

    std::string command, name;
    std::istringstream iss(args);

    // Get the command and channel name
    std::getline(iss, command, '#');
    std::getline(iss, name);

    // Get the channel object by name
    Channels *channel = server->getChannelByName(name);

    if (channel == nullptr) {
        // Channel does not exist
        cout << "Channel " << name << " does not exist" << endl;
        return false;
    } else {
        // Channel exists, remove the client from it
        channel->removeUser(client); // Assuming Channels has a removeUser method
        cout << "Client " << clientFd << " removed from the existing channel " << name << endl;
    }

    return true;
}