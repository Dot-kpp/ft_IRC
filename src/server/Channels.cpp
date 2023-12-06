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

std::string Channels::getChannelName() const { return name; }

std::string Channels::getTopic() const { return topic; }

void Channels::setTopic(std::string topic) { this->topic = topic; }

Channels::~Channels(){}