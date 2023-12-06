#include "../../inc/server/Channels.hpp"

using std::cin;
using std::cout;
using std::endl;

Channels::Channels() {}

Channels::Channels(Channels const *copy)
{
      *this = copy;
}

Channels &Channels::operator=(Channels const &rhs)
{
      if (this != &rhs)
      {
      }
      return (*this);
}

Channels::Channels(int channelId, std::string name)
{
	  this->channelId = channelId;
	  this->name = name;
	  this->topic = "[empty]";
	  this->hasKey = false;
	  this->hasInviteOnly = false;
	  this->hasTopicRestriction = false;
	  this->hasUserLimit = false;
	  this->userLimit = 0;
}

//void Channels::addClient(Client* client) {
//	// Check if the client is not already in the channel
//	if (std::find(clients.begin(), clients.end(), client) == clients.end()) {
//		clients.push_back(client);
//	}
//}

void Channels::addUsers(Client* user, int roleId) {
	users[user] = roleId;
}

void Channels::removeUser(Client* user) {
	users.erase(user);
}

int Channels::getUserRole(Client* user) const {
	std::map<Client*, int>::const_iterator it = users.find(user);
	return (it != users.end()) ? it->second : -1; // Return -1 if user not found
}

void Channels::setChannelName(std::string name) { this->name = name; }

const std::map<Client *, int> &Channels::getUsers() const {
	return users;
}

// Will toggle the bool status (true/false) depending on the current status
void Channels::toggleChannelKey() { this->hasKey = !hasKey; }

void Channels::toggleInviteOnly() { this->hasInviteOnly = !hasInviteOnly; }

void Channels::toggleTopicRestriction() { this->hasTopicRestriction = !hasTopicRestriction; }

void Channels::toggleUserLimit() { this->hasUserLimit = !hasUserLimit; }

void Channels::setUserLimit(int limit) { this->userLimit = limit; }

void Channels::setKey(std::string key) { this->key = key; }

std::string Channels::getChannelName() const { return name; }

std::string Channels::getTopic() const { return topic; }

void Channels::setTopic(std::string topic) { this->topic = topic; }

Channels::~Channels(){}

void Channels::promoteUser(const std::string &nickname) {
	for (std::map<Client*, int>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->second == 1) {
			std::cout << it->first << " is already promoted to the highest role." << std::endl;
		}
		if (it->first->getNickName() == nickname) {
			it->second = 1; // Promote the user to roleId 1 (operator)
			std::cout << it->first->getNickName() << " is promoted to the highest role." << std::endl;
			return;
		}
	}
}

void Channels::demoteUser(const std::string &nickname) {
	for (std::map<Client*, int>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->second == 2) {
			std::cout << it->first << " is already demoted to the lowest role." << std::endl;
		}
		if (it->first->getNickName() == nickname) {
			it->second = 2; // Promote the user to roleId 2 (operator)
			return;
		}
	}
}
