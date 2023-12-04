#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "../Client.hpp"
#include <iostream>
#include <vector>
#include <map>

class Client;

/* Channel MODES
 * +/-i: Invite only
 * +/-t: Topic restriction
 * +/-k: Key
 * +/-o: Operator privilege
 * +/-l: User limit
  * */

class Channels
{
private:
    int channelId;
    std::string name;
    std::string topic;
    std::vector<Client *> clients;
    
public:
    Channels();
    Channels(int channelId, std::string name);
    Channels(Channels const *copy);
    Channels &operator=(Channels const &rhs);
    ~Channels();

    int getChannelId() const { return channelId; }
	std::string getChannelName() const;
	std::string getTopic() const;
	const std::map<Client *, int> &getUsers() const;
//	const std::vector<Client *> &getClients() const; // Get users in channel
	void setTopic(std::string topic);

//    void addClient(Client *client);
//    void removeClient(Client *client) { clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end()); }

	void addUsers(Client* user, int roleId);
	void removeUser(Client* user);
	int getUserRole(Client* user) const;

	void toggleChannelKey();
	void toggleInviteOnly();
	void toggleTopicRestriction();

	//Debug
	bool getHasKey() const { return hasKey; }
	bool getInviteOnly() const { return hasInviteOnly; }
	bool getTopicRestriction() const { return hasTopicRestriction; }

};

std::string trim(const std::string& str) ;

#endif