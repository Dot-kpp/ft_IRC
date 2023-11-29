/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:43:03 by acouture          #+#    #+#             */
/*   Updated: 2023/11/27 15:29:09 by acouture         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../inc/server/Server.hpp"
#include "../../inc/commands/CommandHandler.hpp"

Server *Server::instance = nullptr;

Server::Server(int port, std::string password) : serverSocket(port), port(port), password(password), running(false), serverName("irc")
{
    instance = this;
    std::cout << "Server created" << std::endl;
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
    std::string greeting = "Hello, client!";
    size_t bytesSent = 0;
    size_t totalSent = 0;
    std::string passwordPrompt = "Please enter the server password: ";
    totalSent = 0;
    while (totalSent < passwordPrompt.size())
    {
        bytesSent = send(clientSocket, passwordPrompt.c_str() + totalSent, passwordPrompt.size() - totalSent, 0);
        if (bytesSent == ((size_t)-1))
        {
            std::cerr << "Could not send password prompt to client" << std::endl;
            close(clientSocket);
            return -1;
        }
        totalSent += bytesSent;
    }
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
    std::cout << "Client " << clientFd << " sent: " << strBuffer << std::endl;
    std::istringstream iss(strBuffer);
    std::string commandName;
    iss >> commandName;

    // Check if the client is registered, if not, only NICK and USER are allowed
    if (!client->getIsRegistered() && commandName != "NICK" && commandName != "USER" && commandName != "CAP LS 302")
    {
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
    ssize_t bytesRead = read(clientFd, buffer, sizeof(buffer) - 1);
    buffer[bytesRead] = '\0';

    std::string strBuffer(buffer);
    if (bytesRead > 0)
    {
        // If the client has not provided a password yet, we check if the message is a password
        if (!clients[clientFd].getHasGoodPassword() && strBuffer.substr(0, 4) == "PASS")
        {
            strBuffer.erase(0, 5);
            if (strBuffer.compare(0, this->password.size(), this->password) == 0)
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
            std::cout << clients[clientFd] << std::endl;
            bool hasUserAndNick = clients[clientFd].getNickName() != "" && clients[clientFd].getUserName() != "" ? true : false;
            if (hasUserAndNick)
            {
                clients[clientFd].setIsRegistered(true);
                Channels &channel = this->getChannelById(0);
                channel.addClient(&clients[clientFd]);
            }
            treatIncomingBuffer(strBuffer, clientFd, &clients[clientFd], hasUserAndNick);
        }
    }
    else if (bytesRead > 512)
    {
        std::cout << "Client " << clientFd << " sent a message that was too long." << std::endl;
        std::string tooLong = ":YourServerName 417 * :Input line was too long. \r\n";
        close(clientFd);
        clients.erase(clientFd);
    }
    else if (bytesRead <= 0)
    {
        std::cout << "Client " << clientFd << " disconnected or error." << std::endl;
        close(clientFd);
        clients.erase(clientFd);
    }
}

void Server::start()
{
    this->running = true;

    // Create kqueue
    int kq = kqueue();
    if (kq == -1)
    {
        std::cerr << "Could not create kqueue" << std::endl;
        return;
    }
    KQueue kqueue(kq);
    // Add server socket to kqueue
    EV_SET(kqueue.getChangeEvent(), serverSocket.getSocketFd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kqueue.getKq(), kqueue.getChangeEvent(), 1, NULL, 0, NULL) == -1)
    {
        std::cerr << "Could not add event to kqueue" << std::endl;
        return;
    }

	//Here is the lines that "create" channels manually (don't forget to subscribe to channel, see line 139)
    this->channel.push_back(Channels(0, "default"));
    this->channel.push_back(Channels(1, "Channel1"));
	Oper();

    while (this->running)
    {
        // Wait for events
        int nev = kevent(kq, NULL, 0, kqueue.getEventList(), 1024, NULL);
        for (int i = 0; i < nev; i++)
        {
            // Get client socket
            int clientFd = kqueue.getEventList()[i].ident;
            // If the client socket is the server socket, it means a new client is trying to connect
            if (clientFd == serverSocket.getSocketFd())
            {
                // Accept new client
                int clientSocket = serverSocket.accept();
                // Add client socket to kqueue
                clients[clientSocket] = Client(clientSocket, false, false);
                EV_SET(kqueue.getChangeEvent(), clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                /* askPassword(clientSocket); */
                kevent(kq, kqueue.getChangeEvent(), 1, NULL, 0, NULL);
            }
            else if (kqueue.getEventList()[i].filter == EVFILT_READ)
            {
                // INCOMING MESSAGE FROM CLIENT
                handleIncomingBuffer(clientFd);
            }
        }
    }
    close(kq);
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

Channels* Server::getChannelByName(const std::string& name) {
	for (std::vector<Channels>::iterator it = channel.begin(); it != channel.end(); ++it) {
		if (it->getChannelName() == name) {
			return &(*it);
		}
	}
	return NULL;  // Channel not found
}

void Server::stop()
{
    this->running = false;
    this->serverSocket.closeSocket();
    this->port = 0;
    std::cout << "Server stopped" << std::endl;
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