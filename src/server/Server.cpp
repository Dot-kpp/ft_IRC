/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:43:03 by acouture          #+#    #+#             */
/*   Updated: 2023/12/04 17:56:01 by acouture         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server/Server.hpp"
#include "../../inc/commands/CommandHandler.hpp"

Server *Server::instance = nullptr;

Server::Server(int port, std::string password) : serverSocket(port), port(port), password(password), running(false), serverName("YourServerName")
{
    instance = this;
    serverSocket.bindSocket();
    serverSocket.listenSocket();
    std::cout << "Server launched" << std::endl;
    std::cout << "Port: " << this->port << std::endl;
    std::cout << "Password: " << this->password << std::endl;
};

Server::~Server()
{
    instance = nullptr;
    std::cout << "Server destroyed" << std::endl;
};

std::string Server::getServerName() const
{
    return (this->serverName);
};

std::string Server::getPassword()
{
    return (this->password);
};

int Server::askPassword(int clientSocket)
{
    std::string askPassword = ":YourServerName 331 " + std::to_string(clientSocket) + " :Please enter your password\r\n";
    send(clientSocket, askPassword.c_str(), askPassword.size(), 0);
    return (0);
}

int Server::treatIncomingBuffer(std::string strBuffer, int clientFd, Client *client, bool hasUserAndNick)
{
    (void)hasUserAndNick;
    CommandHandler commandHandler;
    if (strBuffer.empty())
    {
        std::string noCommandError = ": 421 " + std::to_string(clientFd) + " :Unknown command";
        send(clientFd, noCommandError.c_str(), noCommandError.size(), 0);
        return -1;
    }

    // Get the command name
    std::istringstream iss(strBuffer);
    std::string commandName;
    std::string msg = iss.str();
    iss >> commandName;
    std::cout << "Client " << clientFd << " sent: " << msg.c_str() << std::endl;

    // Check if the client is registered, if not, only NICK and USER are allowed
    if (!client->getIsRegistered() && (!commandName.compare(0, 4, "USER") && !commandName.compare(0, 4, "NICK") && !msg.compare(0, 11, "CAP LS 302")))
    {
        std::cout << msg.c_str() << std::endl;
        std::string error = ": 451 " + std::to_string(clientFd) + " :You have not registered\r\n";
        send(clientFd, error.c_str(), error.size(), 0);
        return -1;
    }

    // Check if the client is already registered, if so, USER is not allowed
    if (client->getIsRegistered() && commandName == "USER")
    {
        std::cout << *client << std::endl;
        std::string error = ": 462 " + std::to_string(clientFd) + " :You may not reregister\r\n";
        send(clientFd, error.c_str(), error.size(), 0);
        return -1;
    }

    // Check if the command is registered and handle it
    if (commandHandler.isCommandRegistered(commandName))
        return commandHandler.handleCommand(commandName, strBuffer, clientFd) ? 0 : -1;

    return 0;
}

void Server::handleIncomingBuffer(int clientFd)
{
    char buffer[512];
    // stores the number of bytes read
    size_t bytesRead = recv(clientFd, buffer, sizeof(buffer), 0);
    buffer[bytesRead] = '\0';

    std::string strBuffer(buffer);
    std::vector<std::string> lines;
    std::istringstream iss(strBuffer);
    std::string line;
    while (std::getline(iss, line))
    {
        lines.push_back(line);
    }
    if (bytesRead > 0)
    {
        for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
        {
            if (it->compare("EXIT") == 0)
                this->stop();
            // If the client has not provided a password yet, we check if the message is a password
            if (!clients[clientFd].getHasGoodPassword() && it->substr(0, 4) == "PASS")
            {
                it->erase(0, 5);
                if (it->compare(0, this->password.size(), this->password) == 0)
                    clients[clientFd].setHasGoodPassword(true);
                else
                {
                    std::cout << "Client " << clientFd << " provided the wrong password." << std::endl;
                    std::string wrongPassword = ":YourServerName 464 * :Password incorrect. \r\n";
                    send(clientFd, wrongPassword.c_str(), wrongPassword.size(), 0);
                }
            }
            // If the client has provided a password, we check if the message is a command
            else if (clients[clientFd].getHasGoodPassword())
            {
                bool hasUserAndNick = clients[clientFd].getUserName() != "" && clients[clientFd].getNickName() != "" ? true : false;
                if (hasUserAndNick)
                {
                    Channels &channel = this->getChannelById(0);
                    channel.addUsers(&clients[clientFd], 2); // 2 is User by Default, 1 is Moderator
                    //                channel.addClient(&clients[clientFd]);
                }
                treatIncomingBuffer(*it, clientFd, &clients[clientFd], hasUserAndNick);
            }
        }
    }
    else
    {
        std::cout << "Client " << clientFd << " disconnected." << std::endl;
        removeClient(clientFd, "Client disconnected from the server.");
    }
    memset(buffer, 0, sizeof(buffer));
}

void Server::start()
{
    int new_events;
    struct kevent change_event[4], event[4];
    struct sockaddr_in client_addr;
    socklen_t client_len;

    // Create kqueue
    int kq = kqueue();
    EV_SET(&change_event[0], serverSocket.getSocketFd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);

    // Register kevent with the kqueue
    if (kevent(kq, change_event, 1, NULL, 0, NULL) == -1)
    {
        perror("kevent");
        exit(1);
    }

    this->channel.push_back(Channels(0, "default"));
    this->channel.push_back(Channels(1, "Channel1"));
    for (;;)
    {
        new_events = kevent(kq, NULL, 0, event, 1, NULL);
        if (new_events == -1)
        {
            perror("kevent");
            exit(1);
        }

        for (int i = 0; i < new_events; i++)
        {
            int event_fd = event[i].ident;

            if (event_fd == serverSocket.getSocketFd())
            {
                // New connection
                std::cout << "New connection" << std::endl;
                int client_fd = serverSocket.acceptConnection(client_addr, client_len);
                if (client_fd == -1)
                {
                    perror("Accept socket error");
                    continue;
                }
                // Add new client socket to kqueue
                EV_SET(&change_event[0], client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                if (kevent(kq, change_event, 1, NULL, 0, NULL) < 0)
                    perror("kevent error");
            }
            else if (event[i].filter == EVFILT_READ)
            {
                // Incoming data on client socket
                handleIncomingBuffer(event_fd);
            }
        }
    }
}

void Server::welcomeClient(int clientFd)
{
    std::string serverName = this->getServerName();
    std::string welcomeMessage = ":YourServerName 001 :Welcome to the baddest IRC network. \r\n";
    send(clientFd, welcomeMessage.c_str(), welcomeMessage.size(), 0);

    // RPL_YOURHOST
    std::string yourHostMessage = ":YourServerName 002 :Your host is badass ft_IRC, running version 0.0.1 \r\n";
    send(clientFd, yourHostMessage.c_str(), yourHostMessage.size(), 0);

    // RPL_CREATED
    std::string createdMessage = ":YourServerName 003 :This server was created Nov 8 2023. \r\n";
    send(clientFd, createdMessage.c_str(), createdMessage.size(), 0);

    // RPL_MYINFO
    int nbOfUsers = this->clients.size();
    std::cout << "nbOfUsers: " << std::to_string(nbOfUsers) << std::endl;
    int nbOfChannels = this->channel.size();
    std::cout << "nbOfChannels: " << std::to_string(nbOfChannels) << std::endl;
    std::string myInfoMessage = ":YourServerName 004 :ft_IRC 0.0.1 nb of users: " + std::to_string(nbOfUsers) + ", nb of channels: " + std::to_string(nbOfChannels) + " . \r\n";
    send(clientFd, myInfoMessage.c_str(), myInfoMessage.size(), 0);

    // RPL_ISUPPORT
    std::string isupportMessage = ":YourServerName 005 <client> <1-13 tokens> :are supported by this server\r\n";
    send(clientFd, isupportMessage.c_str(), isupportMessage.size(), 0);

    std::cout << "Client " << clientFd << " is now authenticated." << std::endl;
}

Channels *Server::getChannelByName(const std::string &name)
{
    for (std::vector<Channels>::iterator it = channel.begin(); it != channel.end(); ++it)
    {
        if (it->getChannelName() == name)
        {
            return &(*it);
        }
    }
    return NULL;
}

void Server::stop()
{
    std::stringstream ss;
    ss << ":" << serverName << " 421 "
       << ": Server shutting down"
       << "\r\n";
    std::string msg = ss.str();

    for (std::map<int, Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
        send(it->first, msg.c_str(), msg.size(), 0);
    this->running = false;
    this->serverSocket.closeSocket();
    this->port = 0;
    this->clients.clear();
    this->channel.clear();
    Server::~Server();
    std::cout << "Server stopped" << std::endl;
    exit(0);
};

Channels &Server::getChannelById(int id)
{
    std::vector<Channels>::iterator it = this->channel.begin();
    while (it != this->channel.end())
    {
        if (it->getChannelId() == id)
            return *it;
        it++;
    }
    return this->channel[0];
};

void Server::removeClient(int clientFd, std::string reason)
{
    if (reason.empty())
        reason = "Client disconnected";
    std::map<int, Client>::iterator it = this->clients.begin();
    while (it != this->clients.end())
    {
        if (it->first == clientFd)
        {
            tellEveryoneButSender(reason, clientFd);
            clients.erase(clientFd);
            close(clientFd);
            break;
        }
        it++;
    }
};

void Server::tellEveryoneButSender(std::string message, int clientFd)
{
    for (std::map<int, Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (it->first != clientFd)
        {
            std::stringstream ss;
            ss << ":" << serverName << " QUIT "
               << ":Quit: " << clients[clientFd].getNickName() << " is exiting the network with the message: Quit: " << message << "\r\n";
            std::string msg = ss.str();
            send(it->first, msg.c_str(), msg.size(), 0);
        }
    }
}

Client* Server::getClientByNickname(const std::string& nickname) {
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->second.getNickName() == nickname) {
			return &(it->second);
		}
	}
	return nullptr;
}