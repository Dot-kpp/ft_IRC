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
	std::string channelName;
    std::string topic;
//    std::vector<Client *> clients;
	std::map<Client*, int> users;
	bool hasInviteOnly;
	bool hasKey;
	std::string key;
	bool hasTopicRestriction;
//	bool hasOperPrivilege;
	bool hasUserLimit;
	int userLimit;

public:
    Channels();
    Channels(int channelId, std::string name);
    Channels(Channels const *copy);
    Channels &operator=(Channels const &rhs);
    ~Channels();

    int getChannelId() const { return channelId; }
	std::string getChannelName() const;
	void setChannelName(std::string name);
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
	void toggleUserLimit();
	void setUserLimit(int limit);
	void setKey(std::string key);
	bool isOperator(Client* user) const;
	void promoteUser(Client *user);
	void demoteUser(Client *user);

	//Debug
	bool getHasKey() const;
	bool getInviteOnly() const;
	bool getTopicRestriction() const;
	bool getUserLimit() const;
	int getUserLimitValue() const;
	std::string getKey() const;
	bool hasUser(Client *user) const;
};

std::string trim(const std::string& str) ;

#endif