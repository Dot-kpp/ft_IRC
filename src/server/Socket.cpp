/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:45:06 by acouture          #+#    #+#             */
/*   Updated: 2023/12/04 14:01:55 by acouture         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server/Socket.hpp"

Socket::Socket(int port) : port(port) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("ERROR setting socket options");
        exit(1);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(this->port);
}

Socket::~Socket() {
    if (sockfd)
        close(sockfd);
}

int Socket::getSocketFd() const {
    return sockfd;
}

void Socket::bindSocket() {
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }
}

void Socket::listenSocket(int backlog) {
    if (listen(sockfd, backlog) < 0) {
        perror("Error on listen");
        exit(1);
    }
}

int Socket::acceptConnection(struct sockaddr_in &client_addr, socklen_t &client_len) const {
    int new_socket = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (new_socket < 0) {
        perror("Error on accept");
    }
    return new_socket;
}

void Socket::closeSocket() {
    close(sockfd);
}
