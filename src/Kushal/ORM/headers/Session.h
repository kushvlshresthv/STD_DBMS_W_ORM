#ifndef SESSION_H
#define SESSION_H
#include <cppconn/statement.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <rttr/variant.h>


using namespace sql;
using namespace nlohmann;
using namespace rttr;

//forward declarations
json getJsonData(std::string fileUrl);


class Session {
private: 
	Statement* statement;
public: 
	Session(Statement* st) : statement{ st } {
		
	}


	void save(variant obj);
	void get();
	void update();
	void commit();
	void rollback();
	void remove();


	~Session() {
		statement->close();
	}
};




inline bool validateJson(std::string fileUrl) {

	json jsonData;
	jsonData = getJsonData(fileUrl);

	//validation logic: the json file must have username, password, and url inside orm-configuration
	if (jsonData.contains("class")) {
		if (jsonData["class"].contains("name") && jsonData["class"].contains("table")) {
			for (int i = 0; i < jsonData["class"]["property"].size(); i++) {
				if (jsonData["class"]["property"][i].contains("id") && jsonData["class"]["property"][i].contains("column")) {
					return true;
				}
			}
		}
		else {
			std::cerr << "the mapping file does not contain 'name' of the class or name of the 'table'";
		}
	}
	else {
		std::cerr << "the mapping file does not contain 'class'" << std::endl;
	}


	std::cerr << "the mapping file does not contain 'id'(primary key) and/or its corresponding column";
	return false;
}



void Session::save(variant obj) {

}



#endif


