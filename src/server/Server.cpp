/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:43:03 by acouture          #+#    #+#             */
/*   Updated: 2023/11/24 16:21:13 by acouture         ###   ########.fr       */
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
    (void)client;
    CommandHandler commandHandler;
    if (strBuffer.empty())
    {
        std::string noCommandError = ": 421 " + std::to_string(clientFd) + " :Unknown command";
        send(clientFd, noCommandError.c_str(), noCommandError.size(), 0);
        return -1;
    }

    std::cout << "Client " << clientFd << " sent: " << strBuffer << std::endl;
    if (strBuffer.substr(0, 4) == "NICK") {
        if (!commandHandler.handleCommand("NICK", strBuffer, clientFd))
            return -1;
    }
    else if (strBuffer.substr(0, 4) == "PING") {
        if (!commandHandler.handleCommand("PING", strBuffer, clientFd))
            return -1;
    }

    return 0;
}

void Server::handleIncomingBuffer(int clientFd)
{
    char buffer[512];
    ssize_t bytesRead = read(clientFd, buffer, sizeof(buffer) - 1);
    buffer[bytesRead] = '\0';

    std::string strBuffer(buffer);
    if (bytesRead > 0)
    {
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
        else if (clients[clientFd].getHasGoodPassword())
            treatIncomingBuffer(strBuffer, clientFd, &clients[clientFd], true);
    }
    else if (bytesRead > 512)
    {
        std::cout << "Client " << clientFd << " sent a message that was too long." << std::endl;
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

    int kq = kqueue();
    if (kq == -1)
    {
        std::cerr << "Could not create kqueue" << std::endl;
        return;
    }
    KQueue kqueue(kq);
    EV_SET(kqueue.getChangeEvent(), serverSocket.getSocketFd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kqueue.getKq(), kqueue.getChangeEvent(), 1, NULL, 0, NULL) == -1)
    {
        std::cerr << "Could not add event to kqueue" << std::endl;
        return;
    }

    while (this->running)
    {
        int nev = kevent(kq, NULL, 0, kqueue.getEventList(), 1024, NULL);
        for (int i = 0; i < nev; i++)
        {
            int clientFd = kqueue.getEventList()[i].ident;
            // CONNECTION SERVER SOCKET
            if (clientFd == serverSocket.getSocketFd())
            {
                int clientSocket = serverSocket.accept();
                clients[clientSocket] = Client(clientSocket, false);
                EV_SET(kqueue.getChangeEvent(), clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
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
