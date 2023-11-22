
#include "../inc/Client.hpp"

Client::Client(Channels &channel, Socket clientSocket, bool hasGoodPassword) : clientSocket(clientSocket), hasGoodPassword(hasGoodPassword), channel(&channel) {};

Client::~Client() {};

void Client::setPassword(std::string password)
{
    this->password = password;
};

void Client::setHasGoodPassword(bool hasGoodPassword)
{
    this->hasGoodPassword = hasGoodPassword;
};

void Client::setClientSocket(Socket clientSocket)
{
    this->clientSocket = clientSocket;
};

Socket Client::getClientSocket()
{
    return (this->clientSocket);
};

std::string Client::getPassword()
{
    return (this->password);
};

bool Client::getHasGoodPassword()
{
    return (this->hasGoodPassword);
};

std::string Client::getNickName()
{
    return (this->nickname);
};

void Client::setNickName(std::string nick)
{
    this->nickname = nick;
};

void Client::subscribeToChannel(Channels* channel)
{
    this->channel = channel;
};