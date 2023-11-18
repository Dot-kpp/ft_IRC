
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Socket.hpp"
#include <iostream>

class Server;
class Socket;

class Client
{
private:
    Socket clientSocket;
    std::string nickname;
    std::string password;
    bool hasGoodPassword;

public:
    Client() : clientSocket(0), password(""), hasGoodPassword(false){};
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
