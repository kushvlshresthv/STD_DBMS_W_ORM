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
	Driver* driver;
	Connection* con;
	Statement* st;
	
public:

	//constructor establishes the connection with the database and gets the Statement object

	SessionFactory(std::string& url, std::string& username, std::string& password) {
		driver = get_driver_instance();
		con = driver->connect(url, username, password);

		if (con) {
			std::cout << "connection successful";
		}
		else {
			std::cout << "connection failed";
		}
	}


	//buildSession passes the Statement object and creates a new Session Object

	Session* buildSession() {
		
	}
	
};



#endif
