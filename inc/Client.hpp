
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server/Server.hpp"
#include "server/Socket.hpp"
#include "server/Channels.hpp"
#include <iostream>

class Server;
class Socket;
class Channels;

class Client
{
private:
    Socket clientSocket;
    std::string nickname;
    std::string password;
    bool hasGoodPassword;
    std::string mode;
    Channels* channel;


public:
    Client() : clientSocket(0), password(""), hasGoodPassword(false) {};
    Client(Channels &channel, Socket clientSocket, bool hasGoodPassword); 
    ~Client();

    void setPassword(std::string password);
    void setHasGoodPassword(bool hasGoodPassword);
    void setClientSocket(Socket clientSocket);
    Socket getClientSocket();
    std::string getPassword();
    bool getHasGoodPassword();
    std::string getNickName();
    void setNickName(std::string nick);
    void subscribeToChannel(Channels* channel);
};

#endif
