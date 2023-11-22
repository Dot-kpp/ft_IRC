
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
    std::string username;
    std::string nickname;
    std::string password;
    bool hasGoodPassword;
    std::string mode;
    Channels* channel;


public:
    Client() : clientSocket(0), password(""), hasGoodPassword(false) {};
    Client(Socket clientSocket, bool hasGoodPassword); 
    ~Client();

    // setters and getters
    void setPassword(std::string password);
    void setHasGoodPassword(bool hasGoodPassword);
    void setClientSocket(Socket clientSocket);
    void setNickName(std::string nick);
    void setUserName(std::string username);
    Socket getClientSocket() const;
    std::string getPassword() const;
    bool getHasGoodPassword() const;
    std::string getNickName() const;
    std::string getUserName() const;
    Channels* getChannel() const;


    // methods
    void subscribeToChannel(Channels* channel);
};

std::ostream &operator<<(std::ostream &o, Client const &rhs);

#endif
