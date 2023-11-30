#include "../../inc/commands/Join.hpp"

using std::cout;
using std::endl;

Join::Join() : command("JOIN") {}

Join::~Join() {}

Join::Join(Join const &src) {
	*this = src;
}

bool Join::execute(Server *server, std::string args, int clientFd) {
    cout << "You are in NAMES execute" << endl;
    if (args.empty() || clientFd < 0){
        cout << "Not enough param or client doesn't exists" << endl;
        return false;
    }

    // Get the Client object associated with clientFd
    Client* client = server->getClientByID(clientFd);
    if (!client) {
        cout << "Client with fd " << clientFd << " does not exist" << endl;
        return false;
    }

    // Check if the channel exists
    std::map<std::string, Channels>::iterator it = server->channels.find(args);
    if (it == server->channels.end()) {
        // Channel does not exist, create it
        Channels newChannel; // Assuming Channels is a class
        newChannel.addClient(client); // Pass the Client object
        server->channels[args] = newChannel;
        cout << "Channel " << args << " created and client " << clientFd << " added to it." << endl;
    } else {
        // Channel exists, add the client to it
        it->second.addClient(client); // Pass the Client object
        cout << "Client " << clientFd << " added to the existing channel " << args << endl;
    }

    return true;
}