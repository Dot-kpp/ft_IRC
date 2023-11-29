#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "../Client.hpp"
#include <iostream>
#include <vector>

class Client;

class Channels
{
private:
    int channelId;
    std::string name;
    std::vector<Client *> clients;

public:
    Channels();
    Channels(int channelId, std::string name);
    Channels(Channels const *copy);
    Channels &operator=(Channels const &rhs);
    ~Channels();

    int getChannelId() const { return channelId; }

    void addClient(Client *client);
    void removeClient(Client *client) { clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end()); }

	// Get the channel name
	std::string getChannelName() const { return name; }

	// Get users in channel
//	std::string getClients() const;
	const std::vector<Client *> &getClients() const;

};

#endif