/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:45:50 by acouture          #+#    #+#             */
/*   Updated: 2023/12/15 14:13:34 by acouture         ###   ########.fr       */
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
    int activeConnections;
    std::string password;
    bool running;
    const std::string serverName;
	std::vector<int> clientFds;

public:
    static Server *instance;

    Server();
    Server(int port, std::string password);
    virtual ~Server();

    std::map<int, Client> clients;
    std::vector<Channels> channel;

    void start();
    void stop();
    std::string getPassword();
    int askPassword(int clientSocket);
    int treatIncomingBuffer(std::string strBuffer, int clientFd, Client *client, bool hasUserAndNick);
    void handleIncomingBuffer(int clientFd);
    void welcomeClient(int clientFd);
    std::string getServerName() const;
	void addClientFd(int clientFd);
	void removeClientFd(int clientFd);
    int getActiveConnections() const;
    void setActiveConnections(int activeConnections);
    static void handleSignal(int signal)
    {
        if (signal == SIGINT || signal == SIGTERM)
        {
            std::cout << "Received signal " << signal << ", closing server..." << std::endl;
            instance->stop();
            exit(0);
        }
    }
	Channels &getChannelById(int id);
	Channels* getChannelByName(const std::string& name);
    void removeClient(int clientFd, std::string reason);
    void tellEveryoneButSender(std::string message, int clientFd);
    Client* getClientByFd(int clientFd);
	Client* getClientByNickname(const std::string& nickname);
	void broadcastToChannel(const std::string& channelName, const std::string& message, int senderFd, std::string nickname);
	void sendMessageToClient(int targetClientFd, const std::string& message, std::string targetNickname, std::string nickname);

};

#endif
