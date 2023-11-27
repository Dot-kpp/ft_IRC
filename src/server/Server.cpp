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
    (void)client;
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
    if (!client->getIsRegistered() && commandName != "NICK" && commandName != "USER")
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
    {
        return commandHandler.handleCommand(commandName, strBuffer, clientFd) ? 0 : -1;
    }

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
                clients[clientFd].subscribeToChannel(0);
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

    this->channel.push_back(Channels(0));
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

void Server::stop()
{
    this->running = false;
    this->serverSocket.closeSocket();
    this->port = 0;
    std::cout << "Server stopped" << std::endl;
};
