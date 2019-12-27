#pragma once

#include <iostream>
#include <exception>

using namespace std;

class GameException : public exception{
public:
	GameException(string errmsg);

	virtual const char* what() const throw();

private:
	string errmsg;
};

