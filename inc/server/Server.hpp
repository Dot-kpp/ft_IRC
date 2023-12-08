/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:45:50 by acouture          #+#    #+#             */
/*   Updated: 2023/12/08 16:16:24 by acouture         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "Channels.hpp"
#include "../Client.hpp"
#include <sstream>
#include <iostream>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <set>

class Client;
class Channels;
class Socket;

class Server
{

private:
    Socket serverSocket;
    int port;
    std::string password;
    bool running;
    const std::string serverName;

public:
    static Server *instance;

    Server();
    Server(int port, std::string password);
    ~Server();

    std::map<int, Client> clients;
    std::vector<Channels> channel;

    void start();
    void stop();
    // parsing server commands
    int askPassword(int clientSocket);
    int treatIncomingBuffer(std::string strBuffer, int clientFd, Client *client);
    void treatPassCommand(std::string strBuffer, int clientFd);
    void handleIncomingBuffer(int clientFd);
    void welcomeClient(int clientFd);
    static void handleSignal(int signal)
    {
        if (signal == SIGINT || signal == SIGTERM)
        {
            std::cout << "Received signal " << signal << ", closing server..." << std::endl;
            instance->stop();
            exit(0);
        }
    }
    // getters
    std::string getServerName() const;
    std::string getPassword();
    Channels &getChannelById(int id);
    Channels *getChannelByName(const std::string &name);
    Client *getClientByFd(int clientFd)
    {
        try
        {
            return &clients.at(clientFd);
        }
        catch (std::out_of_range &e)
        {
            return nullptr;
        }
    }
    // general server commands
    void removeClient(int clientFd, std::string reason);
    void tellEveryoneButSender(std::string message, int clientFd);
};

#endif
