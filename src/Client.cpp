
#include "../inc/Client.hpp"

Client::Client(Socket clientSocket, bool hasGoodPassword) : clientSocket(clientSocket), hasGoodPassword(hasGoodPassword){};

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