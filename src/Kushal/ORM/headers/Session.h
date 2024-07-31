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
#include <cstddef>
#include <cppconn/resultset.h>


using namespace sql;
using namespace nlohmann;
using namespace rttr;

//forward declarations
json getJsonData(std::string fileUrl);


class Session {
private: 
	Statement* statement;
	ResultSet* resultSet;
public: 
	Session(Statement* st) : statement{ st } {
		statement->execute("set autocommit = 0");
	}


	void save(variant obj);
	variant get(std::string className, variant primaryKey);
	void update();
	void commit();
	void rollback();
	void remove();


	~Session() {
		statement->close();
	}
};



//throws runtime_error if the mapping file is not validated

inline bool validateJson(const json& jsonData) {

	//validation logic: the json file must have class name and table name
	if (jsonData.contains("class")) {
		if (jsonData["class"].contains("name") && jsonData["class"].contains("table")) {

			//the json file must have id or dataMember(name)
			for (int i = 0; i < jsonData["class"]["property"].size(); i++) {
				if (jsonData["class"]["property"][i].contains("id")) {
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


	throw std::runtime_error("the mapping file does not contain 'id'(primary key)");
	return false;
}



inline void Session::save(variant mainObject) {
	type type_mainObject = mainObject.get_type();
	json jsonData = getJsonData("./src/Kushal/STD_DBMS/json/" + type_mainObject.get_name().to_string() + ".mapping.json");

	if (validateJson(jsonData)) {
	

		std::string tableName = jsonData["class"]["table"].get<std::string>();  //get the table name from json file
		

		//vector allows dynamic size
		std::vector <std::string> dataMembers;
		std::vector <std::string> columnNamesForDataMembers;
		std::vector <std::string> columnNamesForPrimaryKeys;
		std::vector <std::string> primaryDataMembers;

		size_t totalData = jsonData["class"]["property"].size();

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
		

		
		//extracting primary column names and non primary column names from json file present inside the property array
		for (const json& item: propertiesInJson) {

			//if both id and column are present, store the column name in columnNamesForPrimaryKeys
			if (item.contains("id") && item.contains("column")) {
				columnNamesForPrimaryKeys.push_back(item["column"].get<std::string>());
			}

			//if only id is prseent, store the column name same as the id name
			else if(item.contains("id") && !(item.contains("column"))) {
				columnNamesForPrimaryKeys.push_back(primaryDataMembers[columnNamesForPrimaryKeys.size()]);

			//if both dataMember and column are present, store the column name in the columnNamesForDataMembers
			} else if (item.contains("name") && item.contains("column")) {
				columnNamesForDataMembers.push_back(item["column"].get<std::string>());
			}

			//if only dataMember is present, store the column name same as the dataMember
			else if(item.contains("name") && !(item.contains("column"))) {
				columnNamesForDataMembers.push_back(dataMembers[columnNamesForDataMembers.size()]);
			}

		}
	

		//maing sql Query Format
		//insert into tableName (primary column names+ non primary column names) values (primary data members + non primary data members)
		std::string sqlQuery = "insert into " + tableName + "(";




		//adding primary column names

		size_t size = columnNamesForPrimaryKeys.size();

		int count = 0;
		count = 0;
		
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
			if (!columnNamesForPrimaryKeys.empty()) {
				sqlQuery = sqlQuery + ", ";
			}

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
				sqlQuery = sqlQuery + "'" + value + "'";
				count++;

				if (!(count == size)) {
					sqlQuery = sqlQuery + ", ";
				}
			}
			else {
				throw std::runtime_error("The data member: " + primaryDataMember + "is not present in " + type_mainObject.get_name());
			}
		}


		//extracting non-primary data members and adding it to the sql query

		if (dataMembers.empty()) {
			sqlQuery = sqlQuery + ") ";
		}
		else {
			if (!primaryDataMembers.empty()) {
				sqlQuery = sqlQuery + ", ";
			}
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



inline void Session::commit() {
	statement->execute("commit");;
}


inline void Session::rollback() {
	statement->execute("rollback");
}


inline variant Session::get(std::string className, variant primaryKey) {
	std::string sqlQuery = "select * from emp1 where eno = 222";

	//first retrieve an empty object of the class

	type type_obj = type::get_by_name(className);
	variant var_obj = type_obj.create();
	


	//get the column names and data member names from the json file
	
	json jsonData = getJsonData("./src/Kushal/STD_DBMS/json/"+className+".mapping.json");

	std::vector <std::string> dataMembers;
	std::vector <std::string> columnNames;

	if (validateJson(jsonData)) {

		//propertiesInJson is an array that contains id/column and name/column pair
		json propertiesInJson = jsonData["class"]["property"];


		//extracting data member names into dataMembers vector

		for (const json& item : propertiesInJson) {
			//item is a single id/column or name/column pair
			if (item.contains("id")) {
				dataMembers.push_back(item["id"].get<std::string>());
			}
			else if (item.contains("name")) {
				dataMembers.push_back(item["name"].get<std::string>());
			}
		}




		//extracting column names into columNames
		for (const json& item : propertiesInJson) {
			if (item.contains("column")) {
				columnNames.push_back(item["column"].get<std::string>());
			}
			else {
				columnNames.push_back(dataMembers[columnNames.size()]);
			}
		}


	}

	



	

	//create the sql query


	//get the result set object and assign the values to the empty object


	property enoProp = type_obj.get_property("eno");

	resultSet = statement->executeQuery(sqlQuery);

	resultSet->next();
	int value = resultSet->getInt("ENO");

	enoProp.set_value(var_obj, value);

	if (resultSet->next()) {
		for (property prop : type_obj.get_properties()) {
			type propType = prop.get_type();

			if (propType == type::get<std::string>()) {
				std::string value = resultSet->getString("demo");
			}
		}
	}


	return var_obj;
}




#endif


