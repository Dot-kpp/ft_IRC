#include "../../inc/Command.hpp"
#include "../../inc/Join.hpp"

Join::Join()
{
}

Join::~Join()
{
}


bool Join::execute(std::string args, int clientFd)
{
    // Parse the channel name from the args string
    std::string channelName = args.substr(5); // Assuming the channel name starts at the 5th character

    // Check if the channel exists
    if (channels.find(channelName) == channels.end())
    {
        // If the channel does not exist, create it
        createChannel(channelName, clientFd);
    }

    // Add the client to the channel
    channels[channelName].addClient(clientFd);

    return true; // Return true if the command was executed successfully
}

void Join::createChannel(std::string channelName, int clientFd)
{
    // Create a new Channel object
    Channel newChannel;

    // Add the client to the new channel
    newChannel.addClient(clientFd);

    // Add the new channel to the channels map
    channels[channelName] = newChannel;
}