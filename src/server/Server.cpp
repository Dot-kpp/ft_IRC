/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:43:03 by acouture          #+#    #+#             */
/*   Updated: 2023/11/22 16:10:35 by acouture         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server/Server.hpp"

Server *Server::instance = nullptr;

Server::Server(int port, std::string password) : serverSocket(port), port(port), password(password), running(false)
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

int Server::parseIncomingBuffer(std::string buffer)
{
    (void)buffer;
    return (0);
}

int Server::treatIncomingBuffer(std::string strBuffer, int clientFd, Client *client)
{
    if (strBuffer.substr(0, 4) == "NICK")
    {
        std::cout << "Received NICK from client " << clientFd << ": " << strBuffer << std::endl;
        // TODO: Add nickname verification and handling
        client->setNickName(strBuffer);
        std::cout << clients[clientFd] << std::endl;
        return 0;
    }
    else if (strBuffer.substr(0, 4) == "USER")
    {
        std::cout << "Received USER from client " << clientFd << ": " << strBuffer << std::endl;
        std::cout << clients[clientFd] << std::endl;
        client->setUserName(strBuffer);
        return 0;
    }
    else if (strBuffer.substr(0, 4) == "MODE")
    {
        std::cout << "Received MODE from client " << clientFd << ": " << strBuffer << std::endl;
        return 0;
    }
    else if (strBuffer.substr(0, 4) == "PART")
    {
        std::cout << "Received PART from client " << clientFd << ": " << strBuffer << std::endl;
        return 0;
    }
    else if (strBuffer.substr(0, 4) == "JOIN")
    {
        std::cout << "Received JOIN from client " << clientFd << ": " << strBuffer << std::endl;
        return 0;
    }
    else if (strBuffer.substr(0, 7) == "PRIVMSG")
    {
        std::cout << "Received PRIVMSG from client " << clientFd << ": " << strBuffer << std::endl;
        return 0;
    }
    else if (strBuffer.substr(0, 4) == "LIST")
    {
        std::cout << "Received LIST from client " << clientFd << ": " << strBuffer << std::endl;
        return 0;
    }
    else if (strBuffer.substr(0, 4) == "QUIT")
    {
        std::cout << "Received QUIT from client " << clientFd << ": " << strBuffer << std::endl;
        return 0;
    }
    else if (strBuffer.substr(0, 4) == "PING")
    {
        std::cout << "Received PING from client " << clientFd << ": " << strBuffer << std::endl;
        return 0;
    }
    else if (strBuffer.substr(0, 4) == "PONG")
    {
        std::cout << "Received PONG from client " << clientFd << ": " << strBuffer << std::endl;
        return 0;
    }
    else if (strBuffer.substr(0, 4) == "KICK")
    {
        std::cout << "Received KICK from client " << clientFd << ": " << strBuffer << std::endl;
        return 0;
    }
    else if (strBuffer.substr(0, 5) == "TOPIC")
    {
        std::cout << "Received TOPIC from client " << clientFd << ": " << strBuffer << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Received unknown command from client " << clientFd << ": " << strBuffer << std::endl;
        return -1;
    }
    return 0;
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
            if (clientFd == serverSocket.getSocketFd())
            {
                int clientSocket = serverSocket.accept();
                clients[clientSocket] = Client(clientSocket, false);
                EV_SET(kqueue.getChangeEvent(), clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                kevent(kq, kqueue.getChangeEvent(), 1, NULL, 0, NULL);
            }
            else if (kqueue.getEventList()[i].filter == EVFILT_READ)
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
                        {
                            clients[clientFd].setHasGoodPassword(true);

                            // RPL_WELCOME
                            std::string welcomeMessage = ":YourServerName 001 :Welcome to the baddest IRC network. \r\n";
                            send(clientFd, welcomeMessage.c_str(), welcomeMessage.size(), 0);

                            // RPL_YOURHOST
                            std::string yourHostMessage = ":YourServerName 002 :Your host is badass ft_IRC, running version 0.0.1 \r\n";
                            send(clientFd, yourHostMessage.c_str(), yourHostMessage.size(), 0);

                            // RPL_CREATED
                            std::string createdMessage = ":YourServerName 003 :This server was created Nov 8 2023. \r\n";
                            send(clientFd, createdMessage.c_str(), createdMessage.size(), 0);

                            // RPL_MYINFO
                            std::string myInfoMessage = ":YourServerName 004 :ft_IRC 0.0.1 (NB OF USER) (NB OF CHANNELS). \r\n";
                            send(clientFd, myInfoMessage.c_str(), myInfoMessage.size(), 0);

                            // RPL_ISUPPORT
                            std::string isupportMessage = ":YourServerName 005 <client> <1-13 tokens> :are supported by this server\r\n";
                            send(clientFd, isupportMessage.c_str(), isupportMessage.size(), 0);

                            std::cout << "Client " << clientFd << " is now authenticated." << std::endl;
                        }
                        else
                        {
                            std::cout << "Client " << clientFd << " provided the wrong password." << std::endl;
                            std::string wrongPassword = ":YourServerName 464 * :Password incorrect. \r\n";
                            send(clientFd, wrongPassword.c_str(), wrongPassword.size(), 0);
                        }
                    }
                    else if (clients[clientFd].getHasGoodPassword())
                    {
                        Channels channel(0);
                        clients[clientFd].subscribeToChannel(&channel);
                        if (treatIncomingBuffer(strBuffer, clientFd, &clients[clientFd]) == -1)
                        {
                            continue;
                        };
                    }
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
