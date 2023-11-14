/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:45:45 by acouture          #+#    #+#             */
/*   Updated: 2023/11/14 16:40:06 by acouture         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

# define MAX_CLIENTS 10
# define BACKLOG 10

class Socket {
public:
    Socket(int port);
    ~Socket();

    bool create();
    bool bind(const int port);
    bool listen() const;
    int accept() const;

private:
    int sockfd;
    sockaddr_in server_addr;
};

#endif // SOCKET_HPP