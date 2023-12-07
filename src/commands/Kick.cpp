#include "../../inc/commands/Kick.hpp"

using std::cout;
using std::endl;

Kick::Kick() : command("KICK") {}

Kick::~Kick() {}

Kick::Kick(Kick const &src) {
    *this = src;
}

bool Kick::execute(Server *server, std::string args, int clientFd) {
    cout << "You are in KICK execute" << endl;

    if (args.empty() || clientFd < 0){
        std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " KICK " + " :No such channel \r\n";
        send(clientFd, replyError.c_str(), replyError.size(), 0);
        return false;
    }

    // Get the Client object associated with clientFd
    Client* client = server->getClientByFd(clientFd);
    if (!client) {
        cout << "Client with fd " << clientFd << " does not exist" << endl;
        return false;
    }

    std::string command, name, target;
    std::istringstream iss(args);

    // Get the command and channel name
    std::getline(iss, command, '#');
    std::getline(iss, name, ' ');
    std::getline(iss, target);

    // Get the channel object by name
    Channels *channel = server->getChannelByName(name);

    if (channel == nullptr) {
        // Channel does not exist
        cout << "Channel " << name << " does not exist" << endl;
        return false;
    } else {
        // Channel exists, find the target client in the channel
        const std::map<Client *, int> &usersMap = channel->getUsers();
        Client* targetClient = nullptr;
        std::map<Client *, int>::const_iterator it;
        for (it = usersMap.begin(); it != usersMap.end(); ++it) {
            if (it->first->getNickName() == target) {
                targetClient = it->first;
                break;
            }
        }
        if (!targetClient) {
            cout << "Target client " << target << " does not exist in the channel" << endl;
            return false;
        }
        // Remove the target client from the channel
        channel->removeUser(targetClient); // Assuming Channels has a removeUser method
        cout << "Client " << target << " removed from the existing channel " << name << " by client " << clientFd << endl;
    }

    return true;
}