#include "GameException.h"

GameException::GameException(string errmsg) {
	this->errmsg = errmsg;
}

const char* GameException::what() const throw() {
	return errmsg.c_str();
}
