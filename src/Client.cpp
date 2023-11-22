
#include "../inc/Client.hpp"

Client::Client(Socket clientSocket, bool hasGoodPassword) : clientSocket(clientSocket), hasGoodPassword(hasGoodPassword) {};

Client::~Client(){};

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

Socket Client::getClientSocket() const
{
    return (this->clientSocket);
};

std::string Client::getPassword() const
{
    return (this->password);
};

bool Client::getHasGoodPassword() const
{
    return (this->hasGoodPassword);
};

std::string Client::getNickName() const
{
    return (this->nickname);
};

Channels *Client::getChannel() const
{
    return (this->channel);
};

void Client::setNickName(std::string nick)
{
    this->nickname = nick;
};

void Client::setUserName(std::string username)
{
    this->username = username;
};

std::string Client::getUserName() const
{
    return (this->username);
};

void Client::subscribeToChannel(Channels *channel)
{
    this->channel = channel;
};

std::ostream &operator<<(std::ostream &o, Client const &rhs)
{
    o << "Client's username: " << rhs.getUserName() << std::endl;
    o << "Client's nickname: " << rhs.getNickName() << std::endl;
    o << "Client hasGoodPassword: " << rhs.getHasGoodPassword() << std::endl;
    o << "Client's Channel: " << rhs.getChannel()->getChannelId() << std::endl;

    return (o);
};