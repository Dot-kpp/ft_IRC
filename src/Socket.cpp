/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:45:06 by acouture          #+#    #+#             */
/*   Updated: 2023/11/14 14:59:48 by acouture         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Socket.hpp"

Socket::Socket() : sockfd(0)
{
    std::cout << "Socket created" << std::endl;
}

Socket::~Socket()
{
    if (this->sockfd)
        close(this->sockfd);
    std::cout << "Socket destroyed" << std::endl;
}

/**
 * Create a socket
 * @return true if the socket was created, false otherwise
*/
bool Socket::create()
{
    // Create socket
    // AF_INET = IPv4 protocol
    // SOCK_STREAM = TCP
    // 0 = IP protocol
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1)
    {
        std::cerr << "Could not create socket" << std::endl;
        return (false);
    }
    std::cout << "Socket created" << std::endl;
    return (true);
}

/**
 * Bind the socket to a port
 * @param port the port to bind to
 * @return true if the socket was bound, false otherwise
*/
bool Socket::bind(const int port)
{
    // Bind socket to port
    this->server_addr.sin_family = AF_INET;
    this->server_addr.sin_addr.s_addr = INADDR_ANY;
    // htons converts a port number in host byte order to a port number in network byte order
    this->server_addr.sin_port = htons(port);
    // bind() assigns the address specified by addr to the socket referred to by the file descriptor sockfd
    if (::bind(this->sockfd, (struct sockaddr *)&this->server_addr, sizeof(this->server_addr)) == -1)
    {
        std::cerr << "Could not bind to port " << port << std::endl;
        return (false);
    }
    std::cout << "Binded to port " << port << std::endl;
    return (true);
}

/**
 * Listen for incoming connections
 * @return true if the socket is listening, false otherwise
*/
bool Socket::listen() const
{
    // Listen for connections
    if (::listen(this->sockfd, BACKLOG) == -1)
    {
        std::cerr << "Could not listen" << std::endl;
        return (false);
    }
    std::cout << "Waiting for connections..." << std::endl;
    return (true);
}

/**
 * Accept an incoming connection
 * @return the socket file descriptor of the new connection
*/
int Socket::accept() const
{
    // Accept incoming connection
    int new_socket;
    int addrlen = sizeof(this->server_addr);
    // Accept incoming connection
    if ((new_socket = ::accept(this->sockfd, (struct sockaddr *)&this->server_addr, (socklen_t *)&addrlen)) == -1)
    {
        std::cerr << "Could not accept connection" << std::endl;
        return (-1);
    }
    std::cout << "Connection accepted" << std::endl;
    return (new_socket);
}