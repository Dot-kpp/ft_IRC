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

std::string Channels::getClients() const {
	std::string users;
	for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
		users += (*it)->getNickName() + " ";
	}

	return users;
}

Channels::~Channels(){}