#include "Command.hpp"

class Join : public Command
{
public:
    Join();
    virtual ~Join();

    virtual bool execute(std::string args, int clientFd) override;
};

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
        // If the channel does not exist, return false
        return false;
    }

    // Add the client to the channel
    channels[channelName].addClient(clientFd);

    return true; // Return true if the command was executed successfully
}