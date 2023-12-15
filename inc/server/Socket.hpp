
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

# define MAX_CLIENTS 10
# define BACKLOG 10

#include <cstring>

class Socket {
public:
    Socket(int port);
    ~Socket();

    int getSocketFd() const;
    void bindSocket();
    void listenSocket(int backlog = 3);
    int acceptConnection(struct sockaddr_in &client_addr, socklen_t &client_len) const;
    void closeSocket();
    void send(const std::string& message);

private:
    int sockfd;
    int port;
    struct sockaddr_in serv_addr;
};


#endif // SOCKET_HPP