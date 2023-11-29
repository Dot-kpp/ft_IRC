# WeedLogger API

# Table of Contents

1. [Overview](#overview)
2. [Key Features](#key-features)
3. [Code Organization](#code-organization)
    - [Root Directory](#root-directory)
    - [Directories](#directories)
    - [Controllers](#controllers)
4. [Technologies Used](#technologies-used)
5. [Authors](#authors)

## Overview

This IRC application, developed in C++, offers a robust and efficient platform for real-time text messaging and communication. It leverages the power and performance of C++ to provide a fast, stable, and scalable chat solution. The application is designed to handle multiple users and channels, ensuring a seamless chatting experience.

## Key Features

- *Multi-Channel Support*: Users can join multiple chat rooms or channels, each dedicated to different topics or discussions.

- *Private Messaging*: Apart from public channels, the application supports private messages between users for confidential communication.

- *Command-Based Interface*: Users can perform various actions like joining channels, leaving channels, and sending messages through simple text commands.

- *Real-Time Communication*: Delivers messages instantly, facilitating live discussions and interactions.

- *Scalability*: Designed to efficiently handle an increasing number of users and messages without compromising performance.

## Code Organization

### Directories

- **inc**: Contains the header files for the source code
    **Client.hpp**:
    - ### **commands**: The commands folder contains all the basic commands for the IRC Chat
     - **Command.hpp**: Contains the class definition for Command. Command is the parent of each commands.
     - **CommandHandler.hpp**: Class definition of the command handler.
     - **Invite.hpp**: Class definition of the invite command.
     - **Join.hpp**: Class definition of the join command.
     - **Kick.hpp**: Class definition of the kick command.
     - **Mode.hpp**: Class definition of the mode command.
     - **Names.hpp**: Class definition of the names command.
     - **Nick.hpp**: Class definition of the nick command.
     - **Notice.hpp**: Class definition of the notice command.
     - **Oper.hpp**: Class definition of the oper command.
     - **Part.hpp**: Class definition of the part command.
     - **Pass.hpp**: Class definition of the pass command.
     - **Ping.hpp**: Class definition of the ping command.
     - **PrivMsg.hpp**: Class definition of the prvmsg command.
     - **Quit.hpp**: Class definition of the quit command.
     - **Topic.hpp**: Class definition of the topic command.

    - ### **server**: The server folder contains all classes related to the server functionnality
        - **Channels.hpp**: Class definition of the channels.
        - **KQueue.hpp**: Class definition of the kqueue
        - **Server.hpp**: Class definition of the server.
        - **Socket.hpp**: Class definition for the sockets.


- **src**: Contains the core functions for the chat
    - **main.cpp**:
        - ```void handleSignal(int signal, Server &server)```
        - ```int main(int ac, char **av)```
    - **Client.cpp**:
        - ```Client::Client(Socket clientSocket, bool hasGoodPassword, bool isRegistered) : clientSocket(clientSocket), hasGoodPassword(hasGoodPassword), isRegistered(isRegistered)```
        - ```Client::~Client(){}```
        - ```void Client::setPassword(std::string password)```
        - ```void Client::setHasGoodPassword(bool hasGoodPassword)```
        - ```void Client::setClientSocket(Socket clientSocket)```
        - ```Socket Client::getClientSocket() const```
        - ```std::string Client::getPassword() const```
        - ```bool Client::getHasGoodPassword() const```
        - ```std::string Client::getNickName() const```
        - ```void Client::setNickName(std::string nick)```
        - ```void Client::setUserName(std::string username)```
        - ```std::string Client::getUserName() const```
        - ```void Client::setIsRegistered(bool isRegistered)```
        - ```bool Client::getIsRegistered() const```
        - ```void Client::setRoleId(int roleId)```
        - ```int Client::getRoleId() const```
        - ```std::ostream &operator<<(std::ostream &o, Client const &rhs)```

    - ### **commands**:
     - **Command.cpp**:
        - ```Command()```
        - ```virtual ~Command()```
        - ```virtual bool execute(Server *server, std::string args, int clientFd) = 0```
     - **CommandHandler.cpp**:
        - ```CommandHandler()```
        - ```bool handleCommand(const std::string &commandName, std::string buffer, int clientFd)```
        - ```bool isCommandRegistered(const std::string &commandName)```
     - **Invite.cpp**:
     - **Join.cpp**:
     - **Kick.cpp**:
     - **Mode.cpp**:
     - **Names.cpp**:
     - **Nick.cpp**:
        - ```Nick()```
        - ```virtual ~Nick()```
        - ```virtual bool execute(Server *server, std::string args, int clientFd)```
        - ```bool parseNickname(std::string nickname, int clientFd)```
     - **Notice.cpp**:
     - **Oper.cpp**:
        - ```Oper()```
        - ```virtual ~Oper()```
        - ```virtual bool execute(Server *server, std::string args, int clientFd)```
     - **Part.cpp**:
     - **Pass.cpp**:
     - **Ping.cpp**:
        - ```Ping()```
        - ```virtual ~Ping()```
        - ```virtual bool execute(Server *server, std::string args, int clientFd)```
     - **PrivMsg.cpp**:
     - **Quit.cpp**:
     - **Topic.cpp**:
    - ### **server**:
        - **Channels.cpp**
            - ```Channels()```
            - ```Channels(int channelId) ```
            - ```Channels(Channels const *copy) ```
            - ```Channels &operator=(Channels const &rhs)```
            - ```~Channels()```
            - ```int getChannelId() const```
            - ```void addClient(Client *client)```
            - ```void removeClient(Client *client)```
        - **KQueue.cpp**
            - ```KQueue(int kq)```
            - ```~KQueue()```
            - ```struct kevent *getEventList()```
            - ```struct kevent *getChangeEvent()```
            - ```void setKq(int kq)```
            - ```int getKq()```
        - **Socket.cpp**
            - ```Socket(int port)```
            - ```~Socket()```
            - ```bool create()```
            - ```bool bind(const int port)```
            - ```bool listen() const```
            - ```int accept() const```
            - ```void closeSocket() const```
            - ```int getSocketFd() const```
        - **Server.cpp**
            - ```Server()```
            - ```Server(int port, std::string password)```
            - ```~Server()```
            - ```void start()```
            - ```void stop()```
            - ```std::string getPassword()```
            - ```int askPassword(int clientSocket)```
            - ```int treatIncomingBuffer(std::string strBuffer, int clientFd, Client *client, bool hasUserAndNick)```
            - ```int parseIncomingBuffer(std::string buffer)```
            - ```void handleIncomingBuffer(int clientFd)```
            - ```void welcomeClient(int clientFd)```
            - ```std::string getServerName() const```
            - ```static void handleSignal(int signal)```
            - ```Channels &getChannelById(int id)```
       


## Authors

- Alexandre Couture
- Jérémie Pilotte
- Clément Senand
