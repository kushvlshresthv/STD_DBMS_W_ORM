#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <string_view>
#include <fstream>
#include "SessionFactory.h"
#include <nlohmann/json.hpp>
#include <memory>

class Configuration {
private:
	std::string url;
	std::string username;
	std::string password;

public: 
	std::string_view getDatabaseUrl() const {
		return url;
	}

	std::string_view getDatabaseUsername() const {
		return username;
	}

	std::string_view getDatabasePassword() const {
		return password;
	}

	void setDatabaseUrl(std::string_view url) {
		this->url = url;
	}

	void setDatabasePassword(std::string_view password) {
		this->password = password;
	}

	void setDatabaseUsername(std::string_view username) {
		this->username = username;
	}

	//load the configuration details from the default location
	void configure();     


	//load the configuration details from the provided file location
	void configure(std::string_view fileUrl);		


	//load the configuration details from the provided 'ofstream' object
	void configure(std::ifstream file);			


	SessionFactory buildSessionFactory() {

	}


	//validate the Json by passing the fileUrl for the json file: 
	bool validateJson(std::string fileUrl);
	
};



using namespace nlohmann;
//returns true if validation is successful
bool Configuration::validateJson(std::string fileUrl) {

	//creating unique_ptr object which stores ifstream* as its data member
	std::unique_ptr<std::ifstream> file(new std::ifstream(fileUrl));


	//checking if file is open
	if (!file->is_open()) {
		std::cerr << "The Configuration file could not be opened" << std::endl;
		return false;
	}


	//creating jsonData and loading the data from file input stream into it
	json jsonData;
	try {
		*file >> jsonData;
	}
	catch (json::parse_error& e) {
		std::cerr << "Configuration file failed to parse:: " << e.what() << std::endl;
		return false;
	}


	//close the file after the data has been loaded to the json
	(*file).close();



	//validation logic: the json file must have username, password, and url inside orm-configuration
	if (jsonData.contains("orm-configuration")) {
		if (jsonData["orm-configuration"].contains("username") && jsonData["orm-configuration"].contains("url") && jsonData["orm-configuration"].contains("password")) {
			return true;
		}
	}

	return false;

}
#endif