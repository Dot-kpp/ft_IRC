
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
    bool isRegistered;
    std::string mode;
    int roleId;

    Channels* channel;

public:
    Client() : clientSocket(0), password(""), hasGoodPassword(false), roleId(3) {};
    Client(Socket clientSocket, bool hasGoodPassword, bool isRegistered); 
    ~Client();

    // setters and getters
    void setPassword(std::string password);
    void setHasGoodPassword(bool hasGoodPassword);
    void setClientSocket(Socket clientSocket);
    void setNickName(std::string nick);
    void setUserName(std::string username);
    void setIsRegistered(bool isRegistered);
    void setRoleId(int roleId);
    Socket getClientSocket() const;
    std::string getPassword() const;
    bool getHasGoodPassword() const;
    std::string getNickName() const;
    std::string getUserName() const;
    bool getIsRegistered() const;
    Channels* getChannel() const;
    int getRoleId() const;


    // methods
    void welcomeClient(int clientFd);
    void subscribeToChannel(Channels* channel);
};

std::ostream &operator<<(std::ostream &o, Client const &rhs);

#endif
