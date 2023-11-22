#include "../../inc/server/Channels.hpp"
using std::cin;
using std::cout;
using std::endl;

Channels::Channels(){};

Channels::Channels(Channels const *copy)
{
      *this = copy;
};
Channels &Channels::operator=(Channels const &rhs)
{
      if (this != &rhs)
      {
      }
      return (*this);
};

Channels::~Channels(){};