#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Channels
{
    private:
        std::map<std::string, Channel> channels; // Assuming Channel is a class

    public:
        // Add find method
        auto find(const std::string& channelName) {
            return channels.find(channelName);
        }

        // Add end method
        auto end() {
            return channels.end();
        }

        // Add subscript operator
        Channel& operator[](const std::string& channelName) {
            return channels[channelName];
        }
    };

#endif