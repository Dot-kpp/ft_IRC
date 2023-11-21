#include "Nick.hpp"
using std::cout;
using std::cin;
using std::endl;
Nick::Nick()
{
std::cout << "Nick constructor is called" << std::endl;
};

Nick::Nick(Nick const *copy)
{
std::cout << "Nick copy constructor is called" << std::endl;
*this = copy;
};
Nick &Nick::operator=(Nick const &rhs)
{
};

Nick::~Nick()
{
      std::cout << "Nick destructor is called" << std::endl;
};