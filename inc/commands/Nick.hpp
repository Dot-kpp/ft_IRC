#ifndef NICK_HPP
#define NICK_HPP

#include "../server/Server.hpp"
#include <iostream>

class Nick
{
private:
    Nick(Nick const *copy);
    Nick &operator=(Nick const &rhs);

public:
    Nick();
    ~Nick();

    int parseNickname(std::string buffer);
};

#endif