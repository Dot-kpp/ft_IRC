
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

void Client::welcomeClient(int clientFd)
{
    std::string welcomeMessage = ":YourServerName 001 :Welcome to the baddest IRC network. \r\n";
    send(clientFd, welcomeMessage.c_str(), welcomeMessage.size(), 0);

    // RPL_YOURHOST
    std::string yourHostMessage = ":YourServerName 002 :Your host is badass ft_IRC, running version 0.0.1 \r\n";
    send(clientFd, yourHostMessage.c_str(), yourHostMessage.size(), 0);

    // RPL_CREATED
    std::string createdMessage = ":YourServerName 003 :This server was created Nov 8 2023. \r\n";
    send(clientFd, createdMessage.c_str(), createdMessage.size(), 0);

    // RPL_MYINFO
    std::string myInfoMessage = ":YourServerName 004 :ft_IRC 0.0.1 (NB OF USER) (NB OF CHANNELS). \r\n";
    send(clientFd, myInfoMessage.c_str(), myInfoMessage.size(), 0);

    // RPL_ISUPPORT
    std::string isupportMessage = ":YourServerName 005 <client> <1-13 tokens> :are supported by this server\r\n";
    send(clientFd, isupportMessage.c_str(), isupportMessage.size(), 0);

    std::cout << "Client " << clientFd << " is now authenticated." << std::endl;
}

std::ostream &operator<<(std::ostream &o, Client const &rhs)
{
    o << "Client's username: " << rhs.getUserName() << std::endl;
    o << "Client's nickname: " << rhs.getNickName() << std::endl;
    o << "Client hasGoodPassword: " << rhs.getHasGoodPassword() << std::endl;
    o << "Client's Channel: " << rhs.getChannel()->getChannelId() << std::endl;

    return (o);
};