#include "../../inc/commands/Part.hpp"

Part::Part() : command("PART") {}

Part::~Part() {}

Part::Part(Part const &src)
{
    *this = src;
}

bool Part::execute(Server *server, std::string args, int clientFd)
{
    std::cout << "You are in PART execute" << std::endl;

    if (args.empty() || clientFd < 0)
    {
        std::string replyError = ":" + server->getServerName() + " 461 " + server->clients[clientFd].getNickName() + " PART :Not enough parameters \r\n";
        send(clientFd, replyError.c_str(), replyError.size(), 0);
        return false;
    }

    Client *client = server->getClientByFd(clientFd);
    if (!client)
        return false;

    std::string name;
    std::istringstream iss(args);

    iss >> name;
    if (name.front() != '#')
    {
        std::string replyError = ":" + server->getServerName() + " 476 " + name + " :Bad Channel Mask\r\n";
        send(clientFd, replyError.c_str(), replyError.size(), 0);
        return false;
    }

    std::string channelName = name;
    Channels *channel = server->getChannelByName(channelName);
    if (channel == nullptr)
    {
        // Channel does not exist
        std::string replyError = ":" + server->getServerName() + " 403 " + server->clients[clientFd].getNickName() + " " + channelName + " :No such channel\r\n";
        send(clientFd, replyError.c_str(), replyError.size(), 0);
        return false;
    }

    if (!channel->hasUser(client))
    {
        std::string replyError = server->clients[clientFd].getNickName() + channelName + ":" + "You're not on that channel\r\n";
        send(clientFd, replyError.c_str(), replyError.size(), 0);
        return false;
    }
    else
    {
        std::string reply = ":" + server->clients[clientFd].getNickName() + " PART " + channelName + "\r\n";
        send(clientFd, reply.c_str(), reply.size(), 0);
        server->broadcastToChannel(channel->getChannelName(), reply, clientFd, client->getNickName());
        channel->removeUser(client);
    }
    return true;
}