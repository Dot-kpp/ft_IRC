/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:43:03 by acouture          #+#    #+#             */
/*   Updated: 2023/11/16 18:01:39 by acouture         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

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
    std::string passwordPrompt = "Please enter the server password:";
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
        int nev = kevent(kq, NULL, 0, kqueue.getEventList(), 32, NULL);
        for (int i = 0; i < nev; i++)
        {
            int clientFd = kqueue.getEventList()[i].ident;
            if (clientFd == serverSocket.getSocketFd())
            {
                int clientSocket = serverSocket.accept();
                clients[clientSocket] = Client(clientSocket, false);
                if (!clients[clientSocket].getHasGoodPassword())
                {
                    askPassword(clientSocket);
                }
                EV_SET(kqueue.getChangeEvent(), clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                kevent(kq, kqueue.getChangeEvent(), 1, NULL, 0, NULL);
            }
            else if (kqueue.getEventList()[i].filter == EVFILT_READ)
            {
                char buffer[1024];
                ssize_t bytesRead = read(clientFd, buffer, sizeof(buffer) - 1);

                if (bytesRead > 0)
                {
                    buffer[bytesRead] = '\0';
                    if (!clients[clientFd].getHasGoodPassword())
                    {
                        if (strncmp(buffer, this->password.c_str(), this->password.size()) == 0)
                        {
                            std::cout << "Client " << clientFd << " provided the correct password." << std::endl;
                            clients[clientFd].setHasGoodPassword(true);
                            std::cout << "Client " << clientFd << " is now authenticated." << std::endl;
                        }
                        else
                        {
                            std::cout << "Client " << clientFd << " provided an incorrect password." << std::endl;
                            close(clientFd);
                            clients.erase(clientFd);
                            continue;
                        }
                    }
                    else
                    {
                        std::cout << "Received data from authenticated client " << clientFd << ": " << buffer << std::endl;
                        // Process the data as required
                    }
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
