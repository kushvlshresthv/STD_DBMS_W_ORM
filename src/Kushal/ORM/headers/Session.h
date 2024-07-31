#ifndef SESSION_H
#define SESSION_H
#include <cppconn/statement.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <rttr/variant.h>
#include <rttr/type>
#include <rttr/property.h>
#include <stdexcept>


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




inline bool validateJson(const json& jsonData) {

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
			throw std::runtime_error("The mapping file does not contain 'name' of the class or name of the 'table");
		}
	}
	else {
		throw std::runtime_error("the mapping file does not contain 'class'");
	}


	throw std::runtime_error("the mapping file does not contain 'id'(primary key) and/or its corresponding column");
	return false;
}



void Session::save(variant mainObject) {
	type type_mainObject = mainObject.get_type();
	json jsonData = getJsonData("./src/Kushal/STD_DBMS/json/" + type_mainObject.get_name().to_string() + ".mapping.json");

	if (validateJson(jsonData)) {
	

		std::string tableName = jsonData["class"]["table"].get<std::string>();  //get the table name from json file
		

		//vector allows dynamic size
		std::vector <std::string> dataMembers;
		std::vector <std::string> columnNamesForDataMembers;
		std::vector <std::string> columnNamesForPrimaryKeys;
		std::vector <std::string> primaryDataMembers;

		int totalData = jsonData["class"]["property"].size();

		//propertiesInJson is an array that contains id/column and name/column pair
		json propertiesInJson = jsonData["class"]["property"];


		//extracting primary data member names and non primary data member names from json file. 
		
		for (const json& item : propertiesInJson) {
			//item is a single id/column or name/column pair
			if (item.contains("id")) {
				primaryDataMembers.push_back(item["id"].get<std::string>());
			} else if (item.contains("name")) {
				dataMembers.push_back(item["name"].get<std::string>());
			}
		}

		
		//extracting primary column names and non primary column names from json file
		for (const json& item: propertiesInJson) {
			if (item.contains("id")) {
				columnNamesForPrimaryKeys.push_back(item["column"].get<std::string>());
			}
			else if (item.contains("name")) {
				columnNamesForDataMembers.push_back(item["column"].get<std::string>());
			}
		}

	

		//maing sql Query Format
		//insert into tableName (primary column names+ non primary column names) values (primary data members + non primary data members)
		std::string sqlQuery = "insert into " + tableName + "(";




		//adding primary column names

		int size = columnNamesForPrimaryKeys.size();
		int count = 0;

		for (std::string& columnName : columnNamesForPrimaryKeys) {
			
			sqlQuery = sqlQuery + columnName;
			count++;
			if (!(count == size)) {
				sqlQuery = sqlQuery + ", ";
			}
		}


		//adding non-primary column names

		if (columnNamesForDataMembers.empty()) {
			sqlQuery = sqlQuery + ") ";
		}
		else {
			sqlQuery = sqlQuery + ", ";
			size = columnNamesForDataMembers.size();
			int count = 0; 
			for (std::string& columnName : columnNamesForDataMembers) {
				sqlQuery = sqlQuery + columnName;
				count++;
				if (count == size) {
					sqlQuery = sqlQuery + ")";
				}
				else {
					sqlQuery = sqlQuery + ", ";
				}
			}
		}

		sqlQuery = sqlQuery + " values (";
		
		



		

		//extracting primary data members and adding it to the sql query
		
		size = primaryDataMembers.size();
		count = 0;

		for (std::string& primaryDataMember : primaryDataMembers) {
			
			//returns the the property mentioned in the argument
			property prop = type_mainObject.get_property(primaryDataMember); 
			if (prop.is_valid()) {
				variant var = prop.get_value(mainObject);
				std::string value = var.to_string();

				sqlQuery = sqlQuery + "'" +  value + "'";
				count++;

				if (!(count == size)) {
					sqlQuery = sqlQuery + ", ";
				}
			}
			else {
				throw std::runtime_error("The data member: " + primaryDataMember +  "is not present in " + type_mainObject.get_name());
			}
		}

		//extracting non-primary data members and adding it to the sql query

		if (dataMembers.empty()) {
			sqlQuery = sqlQuery + ") ";
		}
		else {
			sqlQuery = sqlQuery + ", ";
			size = dataMembers.size();
			count =0;
			for (std::string& dataMember : dataMembers) {
				property prop = type_mainObject.get_property(dataMember);

				if (prop.is_valid()) {
					variant var = prop.get_value(mainObject);
					std::string value = var.to_string();
					sqlQuery = sqlQuery + "'"+  value + "'";
					count++;
					if (count == size) {
						sqlQuery = sqlQuery + ")";
					}
					else {
						sqlQuery = sqlQuery + ", ";
					}

				}
				else {
					throw std::runtime_error("The data member: " + dataMember + "is not present in " + type_mainObject.get_name());
				}
			}
		}
		std::cout << sqlQuery << std::endl;
		statement->executeUpdate(sqlQuery);
	
	} else {
		throw std::runtime_error("json file could not be validated");
	}
}



#endif


