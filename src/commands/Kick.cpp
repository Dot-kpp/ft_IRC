#include "../../inc/commands/Kick.hpp"

using std::cout;
using std::endl;

Kick::Kick() : command("KICK") {}

Kick::~Kick() {}

Kick::Kick(Kick const &src) {
    *this = src;
}

bool Kick::execute(Server *server, std::string args, int clientFd) {
    if (args.empty() || clientFd < 0){
        std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " KICK :Not enough parameters \r\n";
        send(clientFd, replyError.c_str(), replyError.size(), 0);
        return false;
    }

    Client* client = server->getClientByFd(clientFd);

    std::string name, target;
    std::istringstream iss(args);

    iss >> name >> target;

    Channels *channel = server->getChannelByName(name);

    if (channel == nullptr) {
        std::string replyError = ":" + server->getServerName() + " 403 " + server->clients[clientFd].getNickName() + " " + name + " :No such channel\r\n";
		send(clientFd, replyError.c_str(), replyError.size(), 0);
        return false;
    } else {
		if (!channel->isOperator(client)) {
			std::string replyError =
					":" + server->getServerName() + " 482 " + server->clients[clientFd].getNickName() + " " + name +
					" :You're not channel operator\r\n";
			send(clientFd, replyError.c_str(), replyError.size(), 0);
			return false;
		}
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
            std::string replyError = ":" + server->getServerName() + " 441 " + server->clients[clientFd].getNickName() + " " + target + " " + name + " :They aren't on that channel\r\n";
            send(clientFd, replyError.c_str(), replyError.size(), 0);
            return false;
        }
        if(client->getNickName() == targetClient->getNickName()) {
            std::string replyError = ":" + server->getServerName() + " 400 " + server->clients[clientFd].getNickName() + " " + name + " :You can't kick yourself, silly goose\r\n";
            send(clientFd, replyError.c_str(), replyError.size(), 0);
            return false;
        }
        channel->removeUser(targetClient); 
        std::string reply = ":" + server->clients[clientFd].getNickName() + " KICK " + name + " " + targetClient->getNickName() + "\r\n";
        send(clientFd, reply.c_str(), reply.size(), 0);
    }
    return true;
}