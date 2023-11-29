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
}

void Channels::addClient(Client* client) {
	// Check if the client is not already in the channel
	if (std::find(clients.begin(), clients.end(), client) == clients.end()) {
		clients.push_back(client);
	}
}

std::string Channels::getClients() const {
	if (clients.empty()) {
		return "No users in the channel.";
	}

	std::string users;
	for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
		users += (*it)->getNickName() + "\n";
	}

	return users;
}


Channels::~Channels(){}