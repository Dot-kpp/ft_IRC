
#include "../../inc/commands/Nick.hpp"

Nick::Nick(){};

Nick::Nick(Nick const *copy)
{
      *this = copy;
};
Nick &Nick::operator=(Nick const &rhs)
{
      if (this != &rhs)
      {
      }
      return (*this);
};

Nick::~Nick(){};