#include "irc.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Usage: ./irc [host] [port]" << std::endl;
		return (1);
	}
	return(0);
}