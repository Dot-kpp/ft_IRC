/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:43:03 by acouture          #+#    #+#             */
/*   Updated: 2023/11/16 15:53:27 by acouture         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(int port, char* password) : serverSocket(port), port(port), password(password), running(false)
{
    std::cout << "Server created" << std::endl;
};

Server::~Server()
{
    std::cout << "Server destroyed" << std::endl;
};

void Server::start()
{
    this->running = true;

    // Initialize kqueue
    int kq = kqueue();
    if (kq == -1)
    {
        std::cerr << "Could not create kqueue" << std::endl;
        return;
    }
    KQueue kqueue(kq);

    // Add server socket to kqueue with the kevent
    // Initialize the change_event
    EV_SET(kqueue.getChangeEvent(), serverSocket.getSocketFd(), EVFILT_READ, EV_ADD, 0, 0, NULL);

    // Add the change_event to the kqueue
    if (kevent(kqueue.getKq(), kqueue.getChangeEvent(), 1, NULL, 0, NULL) == -1)
    {
        std::cerr << "Could not add event to kqueue" << std::endl;
        return;
    }

    while (this->running)
    {
        std::cout << "In while loop" << std::endl;
        int nev = kevent(kq, NULL, 0, kqueue.getEventList(), 32, NULL);
        for (int i = 0; i < nev; i++)
        {
            int clientFd = kqueue.getEventList()[i].ident;
            if (clientFd == serverSocket.getSocketFd())
            {
                std::cout << "New connection from " << clientFd << std::endl;
                if (kqueue.getEventList()[i].flags & EV_ERROR)
                {
                    std::cerr << "Error on server socket" << std::endl;
                    continue;
                }

                // Accept new connection
                int clientSocket = serverSocket.accept();
                std::string string = "allo";

                send(clientFd, string.c_str(), string.size(), 0);

                // Read password from client
                char passwordBuffer[256];
                ssize_t bytesRead = read(clientSocket, passwordBuffer, sizeof(passwordBuffer) - 1);
                std::cout << bytesRead << std::endl;
                if (bytesRead > 0)
                {
                    passwordBuffer[bytesRead] = '\0';
                    if (strcmp(passwordBuffer, this->password) == 0)
                    {
                        // Correct password, add client socket to kqueue with the kevent
                        EV_SET(kqueue.getChangeEvent(), clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                        kevent(kq, kqueue.getChangeEvent(), 1, NULL, 0, NULL);
                    }
                    else
                    {
                        close(clientSocket);
                    }
                }
                else
                {
                    close(clientSocket);
                }
            }
            else if (kqueue.getEventList()[i].filter == EVFILT_READ)
            {
                char buffer[1024];
                ssize_t bytesRead = read(clientFd, buffer, sizeof(buffer));

                if (bytesRead > 0)
                {
                    // Process incoming data
                    std::cout << "Received " << bytesRead << " bytes from " << clientFd << std::endl;
                }
                else if (bytesRead == 0)
                {
                    close(clientFd);
                }
                else
                {
                    std::cerr << "Read error on client socket " << clientFd << std::endl;
                }
            }
        }
    }
    close(kq);
}

void Server::stop()
{
    this->running = false;
};
