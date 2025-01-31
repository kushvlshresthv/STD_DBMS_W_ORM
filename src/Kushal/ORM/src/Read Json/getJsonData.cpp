#include <string>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

//returns json data from the provided file location
using namespace nlohmann;

json getJsonData(std::string fileUrl) {
	//creating unique_ptr object which stores ifstream* as its data member
	std::unique_ptr<std::ifstream> file(new std::ifstream(fileUrl));


	//checking if file is open
	if (!file->is_open()) {
		throw std::runtime_error("The json file could not be opened");
	}


	//creating jsonData and loading the data from file input stream into it
	json jsonData;
	try {
		*file >> jsonData;
	}
	catch (json::parse_error& e) {
		throw std::runtime_error("The json file could not be parsed");
	}

	return jsonData;
}