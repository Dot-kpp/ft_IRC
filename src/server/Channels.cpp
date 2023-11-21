#include "../../inc/server/Channels.hpp"
using std::cout;
using std::cin;
using std::endl;
Channels::Channels()
{
std::cout << "Channels constructor is called" << std::endl;
};

Channels::Channels(Channels const *copy)
{
std::cout << "Channels copy constructor is called" << std::endl;
*this = copy;
};
Channels &Channels::operator=(Channels const &rhs)
{
};

Channels::~Channels()
{
      std::cout << "Channels destructor is called" << std::endl;
};