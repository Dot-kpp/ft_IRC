/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:45:50 by acouture          #+#    #+#             */
/*   Updated: 2023/11/16 15:52:26 by acouture         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "KQueue.hpp"
#include <iostream>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

class Server {
public:
    Server(int port, char* password);
    ~Server();

    void start();
    void stop();

private:
    Server();
    Socket serverSocket;
    int port;
    char* password;
    bool running;

};

#endif 
