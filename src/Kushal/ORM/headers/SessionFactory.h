#ifndef SESSIONFACTORY_H
#define SESSIONFACTORY_H

#include <cppconn/statement.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <Session.h>
#include <stdexcept>


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

	SessionFactory(std::string url, std::string username, std::string password) {
		driver = get_driver_instance();
		con = driver->connect(url, username, password);


		if (con && con->isValid()) {
			st = con->createStatement();
			//std::cout << "connection established successfully" << std::endl;
		}
		else {
			std::cout << "conneciton failed"<<std::endl;
			throw std::runtime_error("connection failed to be established with the database");
		}
	}
	
public:

	//returns the SessionFactory Instance
	//if this method is executed first time, SessionFactory instance is created and returned
	//if this method is executed after that, previous SessionFactory instance is returned

	static SessionFactory* getSessionFactoryInstance(const std::string& url, const std::string& username, const std::string& password) {
		if (sessionFactory ) {
			return sessionFactory;
		}
		else {
			sessionFactory = new SessionFactory(url, username, password);
			return sessionFactory;
		}
	}


	//buildSession passes the Statement object and creates a new Session Object
	Session* openSession() {
		return new Session(con->createStatement());
	}


	//deleting copy constructor and assignment operator
	SessionFactory(SessionFactory& sf) = delete;
	SessionFactory& operator = (const SessionFactory&) = delete;


	~SessionFactory() {
		con->close();
	}
	
};

#endif
