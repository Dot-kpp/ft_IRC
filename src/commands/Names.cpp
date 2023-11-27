#include "../../inc/commands/Names.hpp"

using std::cout;
using std::endl;

Names::Names() {}

Names::~Names() {}

Names::Names(Names const &src) {
	*this = src;
}

bool Names::execute(std::string args, int clientFd) {
	if (args.empty() || clientFd < 0){
		cout << "Not enough param or client doesn't exists" << endl;
		return false;
	}
	//For each channel, send the list of users


	return true;
}
