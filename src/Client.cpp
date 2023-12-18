
#include "../inc/Client.hpp"

Client::Client() : clientSocket(0), password(""), hasGoodPassword(false), roleId(3) {};

Client::Client(Socket clientSocket, bool hasGoodPassword, bool isRegistered) : clientSocket(clientSocket), hasGoodPassword(hasGoodPassword), isRegistered(isRegistered)
{
    this->roleId = 3;
};

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

/* Channels *Client::getChannel() const
{
    return (this->channel);
}; */

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

void Client::setIsRegistered(bool isRegistered)
{
    this->isRegistered = isRegistered;
};

bool Client::getIsRegistered() const
{
    return (this->isRegistered);
};

void Client::setRoleId(int roleId)
{
    this->roleId = roleId;
};

int Client::getRoleId() const
{
    return (this->roleId);
};

int Client::getClientFd() const
{
    return (this->clientFd);
};

void Client::setClientFd(int fd)
{
    this->clientFd = fd;
};

/* void Client::subscribeToChannel(Channels *channel)
{
    this->channel = channel;
}; */

std::ostream &operator<<(std::ostream &o, Client const &rhs)
{
    o << "Client is registered: " << rhs.getIsRegistered() << std::endl;
    o << "Client hasGoodPassword: " << rhs.getHasGoodPassword() << std::endl;
    if (!rhs.getUserName().empty())
        o << "Client's username: " << rhs.getUserName() << std::endl;
    else if (!rhs.getNickName().empty())
        o << "Client's nickname: " << rhs.getNickName() << std::endl;
    o << "Client Role: " << rhs.getRoleId() << std::endl;
    return (o);
}

void Client::sendMessage(const std::string& message) {
    clientSocket.send(message);
}