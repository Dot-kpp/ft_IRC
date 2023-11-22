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
    std::vector<Client *> clients;

public:
    Channels();
    Channels(int channelId) { this->channelId = channelId; };
    Channels(Channels const *copy);
    Channels &operator=(Channels const &rhs);
    ~Channels();

    void addClient(Client *client) { clients.push_back(client); }
    void removeClient(Client *client) { clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end()); }
};

#endif