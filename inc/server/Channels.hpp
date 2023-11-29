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
    std::string topic;
    std::vector<Client *> clients;

public:
    Channels();
    Channels(int channelId, std::string name);
    Channels(Channels const *copy);
    Channels &operator=(Channels const &rhs);
    ~Channels();

    int getChannelId() const { return channelId; }
	std::string getChannelName() const;
	std::string getTopic() const;
	const std::vector<Client *> &getClients() const; // Get users in channel
	void setTopic(std::string topic);

    void addClient(Client *client);
    void removeClient(Client *client) { clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end()); }

};

std::string trim(const std::string& str) ;

#endif