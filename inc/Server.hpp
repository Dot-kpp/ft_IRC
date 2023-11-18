/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:45:50 by acouture          #+#    #+#             */
/*   Updated: 2023/11/18 16:07:47 by acouture         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "KQueue.hpp"
#include "Client.hpp"
#include <iostream>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <map>

class Client;
class Socket;

class Server
{
public:
    static Server *instance;

    Server(int port, std::string password);
    ~Server();

    std::map<int, Client> clients;
    void start();
    void stop();
    std::string getPassword();
    int askPassword(int clientSocket);
    int treatIncomingBuffer(std::string strBuffer, int clientFd);
    static void handleSignal(int signal)
    {
        if (signal == SIGINT || signal == SIGTERM)
        {
            std::cout << "Received signal " << signal << ", closing server..." << std::endl;
            instance->stop();
            exit(0);
        }
    }

private:
    Server();
    Socket serverSocket;
    int port;
    std::string password;
    bool running;
};

#endif
