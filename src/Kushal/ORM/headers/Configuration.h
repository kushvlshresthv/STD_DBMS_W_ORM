#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <string_view>
#include <fstream>
#include "SessionFactory.h"
#include <nlohmann/json.hpp>
#include <memory>

#define DEFAULT_LOCATION "./src/Kushal/STD_DBMS/json/configuration.orm.json"


using namespace nlohmann;


//forward declarations
json getJsonData(std::string fileURL);



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
	void configure(std::string fileUrl);		


	//load the configuration details from the provided 'ofstream' object
	void configure(std::ifstream& file);			


	SessionFactory* buildSessionFactory() {
		return SessionFactory::getSessionFactoryInstance(url, username, password);
	}


	//validate the Json by passing the fileUrl for the json file: 
	bool validateJson(json jsonData);
	
};




//reads url, username, and password from default location

inline void Configuration::configure() {
	
	//get jsonData from the default location
	json jsonData = getJsonData(DEFAULT_LOCATION);

	//validate and extract url, username, and password
	if (validateJson(jsonData)) {
		url = jsonData["orm-configuration"]["url"].get<std::string>();
		username = jsonData["orm-configuration"]["username"].get<std::string>();
		password = jsonData["orm-configuration"]["password"].get<std::string>();			
	}
	else {
		std::cerr << "Configuration file is not in proper format and connection could not be established with the database" << std::endl;
		std::terminate();
	}
}




//reads url, username, and password from the given location

inline void Configuration::configure(std::string fileUrl) {

	json jsonData = getJsonData(fileUrl);
	if (validateJson(jsonData)) {
		url = jsonData["orm-configuration"]["url"].get<std::string>();
		username = jsonData["orm-configuration"]["username"].get<std::string>();
		password = jsonData["orm-configuration"]["password"].get<std::string>();

	}
	else {
		std::cerr << "Configuration file is not in proper format and connection could not be established with the database" << std::endl;
		std::terminate();
	}

}




//reads url, username, and password from the given filestream

inline void Configuration::configure(std::ifstream& file) {
	json jsonData;

	try {
		file >> jsonData;
	}
	catch (json::parse_error& e) {
		std::cerr << "Configuration file failed to parse::" << std::endl;
		std::terminate();
	}

	if (validateJson(jsonData)) {
		url = jsonData["orm-configuration"]["url"].get<std::string>();
		username = jsonData["orm-configuration"]["username"].get<std::string>();
		password = jsonData["orm-configuration"]["password"].get<std::string>();

	}
	else {
		std::cerr << "Configuration file is not in proper format and connection could not be established with the database" << std::endl;
		std::terminate();
	}
}







//returns true if validation is successful

inline bool Configuration::validateJson(json jsonData) {

	//validation logic: the json file must have username, password, and url inside orm-configuration
	if (jsonData.contains("orm-configuration")) {
		if (jsonData["orm-configuration"].contains("username") && jsonData["orm-configuration"].contains("url") && jsonData["orm-configuration"].contains("password")) {
			return true;
		}
	}
	return false;
}



#endif