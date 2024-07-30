#ifndef SESSIONFACTORY_H
#define SESSIONFACTORY_H

#include <cppconn/statement.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <string_view>
#include <Session.h>

using namespace sql;
class SessionFactory {
private:
	Statement* st;
	
public:

	//constructor establishes the connection with the database and gets the Statement object

	SessionFactory(std::string_view url, std::string_view username, std::string_view password) {

	}


	//buildSession passes the Statement object and creates a new Session Object

	Session* buildSession() {
		
	}
	
};



#endif
