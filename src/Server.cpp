/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acouture <acouture@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:43:03 by acouture          #+#    #+#             */
/*   Updated: 2023/11/14 16:44:44 by acouture         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(int port) : serverSocket(port), port(port), running(false)
{
    std::cout << "Server created" << std::endl;
};

Server::~Server()
{
    std::cout << "Server destroyed" << std::endl;
};

void Server::start()
{
    this->running = true;
    while (this->running)
    {
        
        std::cout << "Server running, waiting for connections..." << std::endl;
        
        int clientSocket = this->serverSocket.accept();
        if (clientSocket == -1)
        {
            std::cerr << "Failed to accept client" << std::endl;
            continue;
        };
        sleep(1);
    };
};

void Server::stop()
{
    this->running = false;
};

