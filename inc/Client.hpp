
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server/Server.hpp"
#include "server/Socket.hpp"
#include "server/Channels.hpp"
#include <iostream>

class Server;
class Socket;
class Channels;

class Client : public Channels
{
private:
    Socket clientSocket;
    std::string nickname;
    std::string password;
    bool hasGoodPassword;
    std::string mode;
    int channelId;

public:
    Client() : clientSocket(0), password(""), hasGoodPassword(false) { this->channelId = 0; };
    Client(Socket clientSocket, bool hasGoodPassword);
    ~Client();

    void setPassword(std::string password);
    void setHasGoodPassword(bool hasGoodPassword);
    void setClientSocket(Socket clientSocket);
    Socket getClientSocket();
    std::string getPassword();
    bool getHasGoodPassword();
    std::string getNickName();
    void setNickName(std::string nick);
};

#endif
