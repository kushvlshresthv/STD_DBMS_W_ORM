#ifndef SESSIONFACTORY_H
#define SESSIONFACTORY_H

#include <cppconn/statement.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <string_view>
#include <Session.h>
#include <exception>
#include "Configuration.h"


using namespace sql;

class SessionFactory {
private:
	Driver* driver;
	Connection* con;
	Statement* st;
	inline static SessionFactory* sessionFactory = nullptr;

	//constructor establishes the connection with the database and gets the Statement object
	//if connection fails, it throws std::exception();
	//constructor is private to make the class a singleton class. 

	SessionFactory(std::string& url, std::string& username, std::string& password) {
		driver = get_driver_instance();
		con = driver->connect(url, username, password);


		if (con && con->isValid()) {
			st = con->createStatement();
			std::cout << "connection successful";
		}
		else {
			std::cout << "conneciton failed";
			throw std::exception("connection failure");
		}
	}
	
public:

	//returns the SessionFactory Instance
	//if this method is executed first time, SessionFactory instance is created and returned
	//if this method is executed after that, previous SessionFactory instance is returned

	static SessionFactory* getSessionFactoryInstance(std::string url, std::string username, std::string password) {
		if (sessionFactory ) {
			return sessionFactory;
		}
		else {
			sessionFactory = new SessionFactory(url, username, password);
			return sessionFactory;
		}
	}


	//buildSession passes the Statement object and creates a new Session Object
	Session* buildSession() {
		
	}
	
};

#endif
