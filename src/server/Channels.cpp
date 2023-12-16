#include "../../inc/server/Channels.hpp"

using std::cin;
using std::cout;
using std::endl;

Channels::Channels() {
	this->topic = "[empty]";
	this->hasKey = false;
	this->key = "";
	this->hasInviteOnly = false;
	this->hasTopicRestriction = false;
	this->hasUserLimit = false;
	this->userLimit = 0;
}

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
	  this->key = "";
	  this->hasInviteOnly = false;
	  this->hasTopicRestriction = false;
	  this->hasUserLimit = false;
	  this->userLimit = 0;
}

Channels::~Channels(){}

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

void Channels::setInviteOnly(std::string str) {
	if (str == "true")
		this->hasInviteOnly = true;

	if (str == "false")
		this->hasInviteOnly = false;
}

void Channels::setTopicRestriction(std::string str) {
	if (str == "true")
		this->hasTopicRestriction = true;

	if (str == "false")
		this->hasTopicRestriction = false;
}

void Channels::toggleUserLimit() { this->hasUserLimit = !hasUserLimit; }

void Channels::setUserLimit(int limit) { this->userLimit = limit; }

void Channels::setKey(std::string key) { this->key = key; }

std::string Channels::getChannelName() const { return name; }

std::string Channels::getTopic() const { return topic; }

bool Channels::getHasKey() const {
	if (this->key != "")
		return true;
	else
		return false;
}
bool Channels::getInviteOnly() const { return hasInviteOnly; }
bool Channels::getTopicRestriction() const { return hasTopicRestriction; }
bool Channels::getHasLimit() const { return hasUserLimit; }
std::string Channels::getKey() const { return key; }
size_t Channels::getUserLimitValue() const { return userLimit; }
bool Channels::hasUser(Client *user) const {
	// Check if the user is in the channel
	return users.find(user) != users.end();
}

void Channels::setTopic(std::string topic) { this->topic = topic; }

void Channels::addInvitedUser(Client *user) {
	invitedUsers.push_back(user);
}

bool Channels::isUserInvited(Client* user) const {
    return std::find(invitedUsers.begin(), invitedUsers.end(), user) != invitedUsers.end();
}

void Channels::promoteUser(Client* user) {
	std::map<Client*, int>::iterator it = users.find(user);
	if (it != users.end()) {
		if (it->second == 1) {
			std::cout << it->first << " is already promoted to the highest role." << std::endl;
		} else {
			it->second = 1; // Promote the user to roleId 1 (operator)
			std::cout << it->first->getNickName() << " is promoted to the highest role." << std::endl;
		}
	}
}

void Channels::demoteUser(Client* user) {
	std::map<Client*, int>::iterator it = users.find(user);
	if (it != users.end()) {
		if (it->second == 2) {
			std::cout << it->first->getNickName() << " is already demoted to the lowest role." << std::endl;
		} else {
			it->second = 2;
			std::cout << it->first->getNickName() << " is demoted to the lowest role." << std::endl;
		}
	}
}

bool Channels::isOperator(Client* user) const {
	std::map<Client*, int>::const_iterator it = users.find(user);
	return (it != users.end() && it->second == 1);
}

std::string Channels::getModes() const {
	std::string modes = "+";
	if (getInviteOnly()) modes += "i";
	if (getTopicRestriction()) modes += "t";
	if (getHasKey()) modes += "k";
	if (getHasLimit()) modes += "l";
	return modes;
}

bool Channels::isUserInChannel(const std::string& nickname) const {
	std::map<Client*, int>::const_iterator it;
	for (it = users.begin(); it != users.end(); ++it) {
		Client* user = it->first;
		if (user->getNickName() == nickname) {
			return true;
		}
	}
	return false;
}