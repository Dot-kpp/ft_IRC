#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "../Client.hpp"
#include <iostream>

class Channels
{
private:
    int channelId;
    Client *clients[1024];

public:
    Channels();
    Channels(int channelId) { this->channelId = channelId; };
    Channels(Channels const *copy);
    Channels &operator=(Channels const &rhs);
    ~Channels();


};

#endif