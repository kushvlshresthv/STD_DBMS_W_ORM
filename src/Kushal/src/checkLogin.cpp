#include <iostream>
#include <exception>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <string_view>
#include <string>

using namespace sql::mysql;
using namespace sql;
Driver* driver;
Connection* con;
Statement* st;
ResultSet* rs;

//this function returns correctPassword if both username and password are correct
//return incorrectPasswrod if the username exists in the database but password is incorrect
//returns userNotExisted if the username does not exist in the database

std::string checkLogin(std::string_view username, std::string_view password) {
	try {
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306/kushaldb", "root", "kushal");
		st = con->createStatement();
		rs = st->executeQuery("select * from Login_Details where uname = '" + static_cast<std::string>(username) + "'");
		if (rs->next()) {
			if (rs->getString("upwd") == static_cast<std::string>(password)) {
				return "correctPasswrd";
			}
			else {
				return "incorrectPassword";
			}
		}
		else {
			return "userNotExisted";
		}
	}
	catch (std::exception& e) {
		std::cout << "an error has occured: " << e.what() << std::endl;
	}
}
