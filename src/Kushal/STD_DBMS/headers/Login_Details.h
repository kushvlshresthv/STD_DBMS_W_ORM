#include <string>
#ifndef LOGIN_DETAILS_H
#define LOGIN_DETAILS_H

class Login_Details {
public: 
	std::string username;
	std::string password;

	Login_Details(std::string uname, std::string pwd)
		:username{ uname }, password{ pwd }
	{

	}

	Login_Details() = default;
};

#endif
