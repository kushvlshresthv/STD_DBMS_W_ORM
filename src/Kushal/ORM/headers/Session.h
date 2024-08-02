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
#include <string_view>
#include "Employee.h"
#include "Address.h"


using namespace sql;
using namespace nlohmann;
using namespace rttr;

//forward declarations
json getJsonData(std::string fileUrl);


class Session {
private: 
	Statement* statement;
	ResultSet* resultSet = nullptr;
public: 
	Session(Statement* st) : statement{ st } {
		statement->execute("set autocommit = 0");
	}
	inline static variant defaultVariant = type::get<int>().get_name();

	void save(variant obj, bool subObject = false , std::string primaryKeyForSubObject = "", std::string mainClassName = "");
	variant get(std::string className, variant primaryKey);
	variant new_get(std::string className, variant primaryKey, variant mainObject, bool subObject = false, std::string newTableName = "", std::string dataMemberName = "");
	void update(variant mainObject);
	void saveOrUpdate(variant mainObject);
	void commit();
	void rollback();
	void remove(variant mainObject);
	void createTable(variant mainObject, std::string tableName);
	variant new_get_v_2(variant mainObject, variant primaryKey,bool subObject = false, std::string newTableName = "", std::string dataMemberName = "");


	~Session() {
		statement->close();
	}
};



//throws runtime_error if the mapping file is not validated
inline bool validateJson(const json& jsonData) {
	
	//validation logic: the json file must have class name and table name
	if (jsonData.contains("class")) {
		int count = 0;

		if (jsonData["class"].is_object() && jsonData["class"].contains("name") && jsonData["class"].contains("table")) {

			//the json file must have id or dataMember(name)
			for (int i = 0; i < jsonData["class"]["property"].size(); i++) {
				if (jsonData["class"]["property"][i].is_object() && jsonData["class"]["property"][i].contains("id")) {
					count++;
				}
			}

			if (count == 1) {
				return true;
			}
			else if (count == 0) {
				throw std::runtime_error("the " +jsonData["class"]["name"].get<std::string>() + " mapping file does not contain 'id'(primary key)");
			}
			else if (count >= 2) {
				throw std::runtime_error("the " + jsonData["class"]["name"].get<std::string>() + " mapping file contains more than one 'id' (primary key)");
			}
		}
		else {
			throw std::runtime_error("mapping file does not define the class object in the json file properly");
		}
	}
	else {
		throw std::runtime_error("mapping file does not contain 'class'");
	}



	return false;
}


//saves the given object to the database table based on the mapping details provided in the json file

namespace oldVersion {
	inline void save(variant mainObject) {
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
				}
				else if (item.contains("name")) {
					dataMembers.push_back(item["name"].get<std::string>());
				}
			}

			////

		//extracting primary column names and non primary column names from json file present inside the property array
			for (const json& item : propertiesInJson) {

				//if both id and column are present, store the column name in columnNamesForPrimaryKeys
				if (item.contains("id") && item.contains("column")) {
					columnNamesForPrimaryKeys.push_back(item["column"].get<std::string>());
				}

				//if only id is prseent, store the column name same as the id name
				else if (item.contains("id") && !(item.contains("column"))) {
					columnNamesForPrimaryKeys.push_back(primaryDataMembers[columnNamesForPrimaryKeys.size()]);

					//if both dataMember and column are present, store the column name in the columnNamesForDataMembers
				}
				else if (item.contains("name") && item.contains("column")) {
					columnNamesForDataMembers.push_back(item["column"].get<std::string>());
				}

				//if only dataMember is present, store the column name same as the dataMember
				else if (item.contains("name") && !(item.contains("column"))) {
					columnNamesForDataMembers.push_back(dataMembers[columnNamesForDataMembers.size()]);
				}

			}


			//making sql Query Format
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
				count = 0;
				for (std::string& dataMember : dataMembers) {
					property prop = type_mainObject.get_property(dataMember);

					if (prop.is_valid()) {
						variant var = prop.get_value(mainObject);
						std::string value = var.to_string();
						sqlQuery = sqlQuery + "'" + value + "'";
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

			//statement->executeUpdate(sqlQuery);

		}
		else {
			throw std::runtime_error("json file could not be validated");
		}
	}
}



//commit() commits our sql changes to the database
inline void Session::commit() {
	statement->execute("commit");;
}


//rollback() rollsback and operation performed int the database from previous commit incase of an error
inline void Session::rollback() {
	statement->execute("rollback");
}

//retrieves data in the form of an object based on the primary key value. 
inline variant Session::get(std::string className, variant primaryKeyValue) {
	std::string sqlQuery;

	//first retrieve an empty object of the class

	type type_obj = type::get_by_name(className);
	variant var_obj = type_obj.create();
	


	//get the column names and data member names from the json file
	
	json jsonData = getJsonData("./src/Kushal/STD_DBMS/json/"+className+".mapping.json");

	std::vector <std::string> dataMembers;
	std::vector <std::string> columnNames;
	std::string primaryKeyColumn;
	std::string tableName;

	if (validateJson(jsonData)) {

		//propertiesInJson is an array that contains id/column and name/column pair
		json propertiesInJson = jsonData["class"]["property"];
		tableName = jsonData["class"]["table"].get<std::string>();

		//extracting data member names into dataMembers vector

		for (const json& item : propertiesInJson) {
			//item is a single id/column or name/column pair
			if (item.contains("id")) {
				dataMembers.push_back(item["id"].get<std::string>());

				//store the primaryKey column for writing sql query
				if (item.contains("column")) {
					primaryKeyColumn = item["column"].get<std::string>();
				}
				else {
					primaryKeyColumn = item["id"].get<std::string>();
				}
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
	sqlQuery = "select * from " + tableName + " where " + primaryKeyColumn +  "= " + "'" + primaryKeyValue.to_string() + "'";
	std::cout << sqlQuery << std::endl;



	//get the result set object and assign the values to the empty object
	

	//execute the sql query and retrieve the resultSet
	resultSet = statement->executeQuery(sqlQuery);

	if (resultSet->next()) {
		int count = 0;
		for (std::string dataMember : dataMembers) {

			//get the property

			property prop = type_obj.get_property(dataMember);
			type propType = prop.get_type();

			//check the data type of property(data member) and execute the necessary code
			if (propType == type::get<std::string>()) {

				std::string value = resultSet->getString(columnNames[count]);
				prop.set_value(var_obj, value);

			}
			else if (propType == type::get<int>()) {

				int value = resultSet->getInt(columnNames[count]);
				prop.set_value(var_obj, value);
			}
			else if (propType == type::get<float>()) {

				float value = (float)resultSet->getDouble(columnNames[count]);
				prop.set_value(var_obj, value);

			}
			count++;
		}

	}
	else {
		throw std::runtime_error("no data found for the given primary key in the database table");
	}
	return var_obj;
}


//updates the old object in the table with new object
inline void Session::update(variant mainObject) {
	
	std::vector<std::string> dataMembers;
	std::vector<std::string> columnNames;
	std::string tableName;
	std::string sqlQuery;
	std::string primaryKeyColumn;
	std::string primaryKeyDataMember;
	type type_mainObject = mainObject.get_type();
	std::string className = type_mainObject.get_name().to_string();

	json jsonData = getJsonData("./src/Kushal/STD_DBMS/json/" + className + ".mapping.json");


	//read the json file to obtains dataMember names and their corresponding column names into vectors
	if (validateJson(jsonData)) {

		//propertiesInJson is an array that contains id/column and name/column pair
		json propertiesInJson = jsonData["class"]["property"];
		tableName = jsonData["class"]["table"].get<std::string>();

		//extracting data member names into dataMembers vector

		for (const json& item : propertiesInJson) {
			//item is a single id/column or name/column pair
			if (item.contains("id")) {
				dataMembers.push_back(item["id"].get<std::string>());
				//storing the primaryKeyDataMember to retrieve old object
				primaryKeyDataMember = item["id"].get<std::string>();

				//store the primaryKey column for writing sql query
				if (item.contains("column")) {
					primaryKeyColumn = item["column"].get<std::string>();
				}
				else {
					primaryKeyColumn = item["id"].get<std::string>();
				}
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



	//retrieve the object based on the primary key value of the main object
	variant var_oldObject;
	type type_oldObject = type::get_by_name(className);
	var_oldObject = type_oldObject.create();   //type_mainObject already created at the start of the method

	



		//create the sql query
	sqlQuery = "select * from " + tableName + " where " + primaryKeyColumn + "= " + "'" + type_mainObject.get_property(primaryKeyDataMember).get_value(mainObject).to_string() + "'";



	//get the result set object and assign the values to the empty object
	

	//execute the sql query and retrieve the resultSet
	resultSet = statement->executeQuery(sqlQuery);
	
	if (resultSet->next()) {
		int count = 0;
		for (std::string dataMember : dataMembers) {

			//get the property

			property prop = type_oldObject.get_property(dataMember);
			type propType = prop.get_type();

			//check the data type of property(data member) and execute the necessary code
			if (propType == type::get<std::string>()) {

				std::string value = resultSet->getString(columnNames[count]);
				prop.set_value(var_oldObject, value);

			}
			else if (propType == type::get<int>()) {

				int value = resultSet->getInt(columnNames[count]);
				prop.set_value(var_oldObject, value);
			}
			else if (propType == type::get<float>()) {

				float value = (float)resultSet->getDouble(columnNames[count]);
				prop.set_value(var_oldObject, value);

			}
			count++;
		}
	}
	else {
		throw std::runtime_error("the data for the given object is not available in the database table for updation");
	}

	
	


	sqlQuery = "update " + tableName + " set ";
	//compare the two objects and generate sql query to update the data members which are updated
	int count = 0; 
	bool flag = true;
	for (std::string dataMember : dataMembers) {
		variant variant_prop_oldObject = type_oldObject.get_property(dataMember).get_value(var_oldObject);   
		variant variant_prop_mainObject = type_mainObject.get_property(dataMember).get_value(mainObject);
		if (variant_prop_oldObject.to_string() != variant_prop_mainObject.to_string()) {
			if (!flag) {
				sqlQuery = sqlQuery + ",";
			}
			flag = false;

			
			sqlQuery = sqlQuery + columnNames[count] + "= " + "'" + variant_prop_mainObject.to_string() + "'";
		}
		
		count++;
	}

	if (flag) return;
	variant variant_primaryKey_mainObject = type_mainObject.get_property(primaryKeyDataMember).get_value(mainObject);
	sqlQuery = sqlQuery + " where " + primaryKeyColumn + " = " + "'" + variant_primaryKey_mainObject.to_string() + "'";

	std::cout << sqlQuery << std::endl;
	statement->executeUpdate(sqlQuery);
}

//updates the old object in the table with new object if the object is already present, else saves the object in the table
inline void Session::saveOrUpdate(variant mainObject) {
	type type_mainObject = mainObject.get_type();
	std::string className = type_mainObject.get_name().to_string();
	std::string tableName;
	std::string primaryKeyColumn;
	std::string primaryKeyDataMember;
	std::string sqlQuery;

	//read the primary data member  and primary key column name from json file

	json jsonData = getJsonData("./src/Kushal/STD_DBMS/json/" + className + ".mapping.json");

	if (validateJson(jsonData)) {
		//propertiesInJson is an array that contains id/column and name/column pair
		json propertiesInJson = jsonData["class"]["property"];
		tableName = jsonData["class"]["table"].get<std::string>();

		//extracting primary key column name and primary key data member name

		for (const json& item : propertiesInJson) {
			//item is a single id/column or name/column pair
			if (item.contains("id")) {
				primaryKeyDataMember = item["id"].get<std::string>();
				if (item.contains("column")) {
					primaryKeyColumn = item["column"].get<std::string>();
				}
				else {
					primaryKeyColumn = item["id"].get<std::string>();
				}
			}
		}

		
		sqlQuery = "select * from " + tableName + " where " + primaryKeyColumn + "= " + "'" + type_mainObject.get_property(primaryKeyDataMember).get_value(mainObject).to_string() + "'";


		resultSet = statement->executeQuery(sqlQuery);
		if (resultSet->next()) {
			update(mainObject);
		}
		else {
			save(mainObject);
		}
		
	}
}

//deletes the object from the database table
inline void Session::remove(variant mainObject) {


	//get the primary key and primary key data member

	type type_mainObject = mainObject.get_type();
	std::string className = type_mainObject.get_name().to_string();
	std::string tableName;
	std::string primaryKeyColumn;
	std::string primaryKeyDataMember;
	std::string sqlQuery;

	//read the primary data member  and primary key column name from json file

	json jsonData = getJsonData("./src/Kushal/STD_DBMS/json/" + className + ".mapping.json");

	if (validateJson(jsonData)) {
		//propertiesInJson is an array that contains id/column and name/column pair
		json propertiesInJson = jsonData["class"]["property"];
		tableName = jsonData["class"]["table"].get<std::string>();

		//extracting primary key column name and primary key data member name

		for (const json& item : propertiesInJson) {
			//item is a single id/column or name/column pair
			if (item.contains("id")) {
				primaryKeyDataMember = item["id"].get<std::string>();
				if (item.contains("column")) {
					primaryKeyColumn = item["column"].get<std::string>();
				}
				else {
					primaryKeyColumn = item["id"].get<std::string>();
				}
			}
		}

	}

	resultSet = statement->executeQuery("select * from " + tableName + " where " + primaryKeyColumn + " = " + "'" + type_mainObject.get_property(primaryKeyDataMember).get_value(mainObject).to_string() + "'");

	if (!resultSet->next()) {
		throw std::runtime_error("there is no data for '" + type_mainObject.get_name().to_string() + "' class with the primary data member value = '" + type_mainObject.get_property(primaryKeyDataMember).get_value(mainObject).to_string() + "' in the table '" + tableName + "'");
	}



	//form an sql query to delete the object from the table
	sqlQuery = "delete from " + tableName + " where " + primaryKeyColumn + " = " + "'" + type_mainObject.get_property(primaryKeyDataMember).get_value(mainObject).to_string() + "'";

	std::cout << sqlQuery << std::endl;

	statement->executeUpdate(sqlQuery);
}



inline void Session::createTable(variant mainObject, std::string tableName) {
	//get the columnNames and dataMemberNames from the json mappping file
}

//new version of save method implemented
inline void Session::save(variant mainObject, bool subObject, std::string primaryKeyForSubObject, std::string mainObjectName) {

	//primaryKeyForSubObject sets the same primary key for main object and sub object
	//mainObjectName is used for table Name



	type type_mainObject = mainObject.get_type();
	json jsonData = getJsonData("./src/Kushal/STD_DBMS/json/" + type_mainObject.get_name().to_string() + ".mapping.json");


	if (validateJson(jsonData)) {

		std::vector<std::string> dataMembers;
		std::vector<std::string> columnNames;
		std::string tableName;
		std::string sqlQuery;
		std::string primaryKeyColumn;
		std::string primaryKeyDataMember;
		type type_mainObject = mainObject.get_type();
		std::string className = type_mainObject.get_name().to_string();

		json jsonData = getJsonData("./src/Kushal/STD_DBMS/json/" + className + ".mapping.json");


		//read the json file to obtains dataMember names and their corresponding column names into vectors
		if (validateJson(jsonData)) {

			//propertiesInJson is an array that contains id/column and name/column pair
			json propertiesInJson = jsonData["class"]["property"];
			
			if (subObject) {
				tableName = mainObjectName + "_" + jsonData["class"]["table"].get<std::string>();
			}
			else {
				tableName = jsonData["class"]["table"].get<std::string>();
			}

			//extracting data member names into dataMembers vector

			for (const json& item : propertiesInJson) {
				//item is a single id/column or name/column pair
				if (item.contains("id")) {
					dataMembers.push_back(item["id"].get<std::string>());
					//storing the primaryKeyDataMember to retrieve old object
					primaryKeyDataMember = item["id"].get<std::string>();

					//store the primaryKey column for writing sql query
					if (item.contains("column")) {
						primaryKeyColumn = item["column"].get<std::string>();
					}
					else {
						primaryKeyColumn = item["id"].get<std::string>();
					}
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


		//making sql Query Format
		//insert into tableName (primary column names+ non primary column names) values (primary data members + non primary data members)

		if (subObject == false) {
			sqlQuery = "insert into " + tableName + "(";
		}
		else {
			sqlQuery = "insert into " + tableName + "( primaryKey, ";
		}





		//adding primary column names


		int count = 0;
		count = 0;

		for (std::string& columnName : columnNames) {
			type data_type_check = type_mainObject.get_property(dataMembers[count]).get_value(mainObject).get_type();
			if (!data_type_check.is_class() || data_type_check == type::get <std::string>()) {
				sqlQuery = sqlQuery + columnName + ",";
			}
			count++;

		}
		sqlQuery.pop_back();
		if (subObject == false) {
			sqlQuery = sqlQuery + ") values (";
		}
		else {
			sqlQuery = sqlQuery + ") values (" + primaryKeyForSubObject + ", ";
		}



		//extracting primary data members and adding it to the sql query




		for (std::string& dataMember : dataMembers) {
			type data_type_check = type_mainObject.get_property(dataMember).get_value(mainObject).get_type();
			if (!data_type_check.is_class() || data_type_check == type::get <std::string>()) {
				//returns the the property mentioned in the argument
				property prop = type_mainObject.get_property(dataMember);
				if (prop.is_valid()) {
					variant var = prop.get_value(mainObject);
					std::string value = var.to_string();
					sqlQuery = sqlQuery + "'" + value + "'" + ",";
					count++;
				}
				else {
					throw std::runtime_error("The data member: " + dataMember + "is not present in " + type_mainObject.get_name());
				}
			}
			else {
				if (subObject == false) {
					save(type_mainObject.get_property(dataMember).get_value(mainObject), true, type_mainObject.get_property(primaryKeyDataMember).get_value(mainObject).to_string(), type_mainObject.get_name().to_string());
				}
				else {
					save(type_mainObject.get_property(dataMember).get_value(mainObject), true, primaryKeyForSubObject,type_mainObject.get_name().to_string());
				}
				
			}
		}
		sqlQuery.pop_back();
		sqlQuery = sqlQuery + ")";
		std::cout << sqlQuery << std::endl;
		statement->executeUpdate(sqlQuery);
	}
	else {
		throw std::runtime_error("json file could not be validated");
	}
}




namespace trial {
	
}

inline variant Session::new_get(std::string className, variant primaryKeyValue,variant main_object, bool subObject, std::string newTableName, std::string dataMemberName) {
	std::string sqlQuery;

	//first retrieve an empty object of the class

	type type_obj = type::get_by_name(className);
	variant var_obj;

	
	if (subObject) {
		variant var_main_object = main_object;
		var_obj = var_main_object.get_type().get_property(dataMemberName).get_value(var_main_object);
		std::cout << var_obj.get_type().get_name();
	}
	else {
		var_obj = main_object;
	}


	
	//get the column names and data member names from the json file

	json jsonData = getJsonData("./src/Kushal/STD_DBMS/json/" + className + ".mapping.json");

	std::vector <std::string> dataMembers;
	std::vector <std::string> columnNames;
	std::string primaryKeyColumn;
	std::string tableName;


	if (validateJson(jsonData)) {

		//propertiesInJson is an array that contains id/column and name/column pair
		json propertiesInJson = jsonData["class"]["property"];
		tableName = jsonData["class"]["table"].get<std::string>();
		if (subObject) {
			tableName = "Employee_With_Address_Address";
		}
		//extracting data member names into dataMembers vector

		for (const json& item : propertiesInJson) {
			//item is a single id/column or name/column pair
			if (item.contains("id")) {
				dataMembers.push_back(item["id"].get<std::string>());

				//store the primaryKey column for writing sql query
				if (item.contains("column")) {
					primaryKeyColumn = item["column"].get<std::string>();
				}
				else {
					primaryKeyColumn = item["id"].get<std::string>();
				}
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
	
	if (subObject) {
		sqlQuery = "select * from " + tableName + " where " + "primaryKey" + "= " + "'" + primaryKeyValue.to_string() + "'";
	}
	else {
		sqlQuery = "select * from " + tableName + " where " + primaryKeyColumn + "= " + "'" + primaryKeyValue.to_string() + "'";
	}

	
	
	std::cout << sqlQuery << std::endl;



	//get the result set object and assign the values to the empty object


	//execute the sql query and retrieve the resultSet
	resultSet = statement->executeQuery(sqlQuery);

	if (resultSet->next()) {
		int count = 0;

		if (subObject == false) {
			
			for (std::string dataMember : dataMembers) {
				//get the type object for primary data Member which may be a class or fundamental data type
				
				type data_type_check = type_obj.get_property(dataMember).get_value(var_obj).get_type();
				
				if (!data_type_check.is_class() || data_type_check == type::get<std::string>()) {
					property prop = type_obj.get_property(dataMember);
					type propType = prop.get_type();

					//check the data type of property(data member) and execute the necessary code
					if (propType == type::get<std::string>()) {
						
						std::string value = resultSet->getString(columnNames[count]);
						prop.set_value(var_obj, value);

					}
					else if (propType == type::get<int>()) {

						int value = resultSet->getInt(columnNames[count]);
						prop.set_value(var_obj, value);
					}
					else if (propType == type::get<float>()) {

						float value = (float)resultSet->getDouble(columnNames[count]);
						prop.set_value(var_obj, value);

					}
					count++;
				}
				else {

					//variant for data member
					variant var_dataMember = type_obj.get_property(dataMember).get_value(var_obj);


					//getting the new table name = MainClass_SubClass
					std::string newTableName = type_obj.get_name().to_string() + "_" + var_dataMember.get_type().get_name().to_string();

					//getting the sub object         //data members class name                //same primary key value      

					variant var_subObject = new_get(data_type_check.get_name().to_string(), primaryKeyValue.to_string(), main_object, true ,newTableName, dataMember);

					var_obj.get_type().get_property(dataMember).set_value(var_obj, var_subObject);
				}
			}
		}

	
		else {

			//get the type of the main object

			for (std::string dataMember : dataMembers) {
				//get the type object for primary data Member which may be a class or fundamental data type

				type data_type_check = type_obj.get_property(dataMember).get_value(var_obj).get_type();

				if (!data_type_check.is_class() || data_type_check == type::get<std::string>()) {
					property prop = var_obj.get_type().get_property(dataMember);
					
					type propType = prop.get_type();

					//check the data type of property(data member) and execute the necessary code
					if (propType == type::get<std::string>()) {
						std::string value = resultSet->getString(columnNames[count]);
						prop.set_value(var_obj, value);

					}
					else if (propType == type::get<int>()) {

						int value = resultSet->getInt(columnNames[count]);
						prop.set_value(var_obj, value);
					}
					else if (propType == type::get<float>()) {

						float value = (float)resultSet->getDouble(columnNames[count]);
						prop.set_value(var_obj, value);

					}
					count++;
				}


				else {
					//variant for data member
					variant var_dataMember = type_obj.get_property(dataMember).get_value(var_obj);

					//getting the new table name = MainClass_SubClass
					std::string newTableName = type_obj.get_name().to_string() + "_" + var_dataMember.get_type().get_name().to_string();

					//getting the sub object         //data members class name                //same primary key value         
					variant var_subObject = new_get(data_type_check.get_name().to_string(), primaryKeyValue.to_string(), main_object, true, newTableName, dataMember);
				}
			}
		}
	}
	else {
		throw std::runtime_error("no data found for the given primary key in the database table");
	}
	std::cout << var_obj.get_type().get_name() << std::endl;
	return var_obj;
}





//new get v2

inline variant Session::new_get_v_2(variant main_object, variant primaryKeyValue, bool subObject, std::string newTableName, std::string dataMemberName) {
	std::string sqlQuery;

	//first retrieve an empty object of the class

	variant var_obj;


	if (subObject) {
		variant var_main_object = main_object;
		var_obj = var_main_object.get_type().get_property(dataMemberName).get_value(var_main_object);
		std::cout << var_obj.get_type().get_name();
	}
	else {
		var_obj = main_object;
	}

	type type_obj = var_obj.get_type();
	std::string className = var_obj.get_type().get_name().to_string();



	//get the column names and data member names from the json file

	json jsonData = getJsonData("./src/Kushal/STD_DBMS/json/" + className + ".mapping.json");

	std::vector <std::string> dataMembers;
	std::vector <std::string> columnNames;
	std::string primaryKeyColumn;
	static std::string tableName = "";


	if (validateJson(jsonData)) {

		//propertiesInJson is an array that contains id/column and name/column pair
		json propertiesInJson = jsonData["class"]["property"];
		tableName = jsonData["class"]["table"].get<std::string>();
		if (subObject) {
			tableName = newTableName;
		}
		//extracting data member names into dataMembers vector

		for (const json& item : propertiesInJson) {
			//item is a single id/column or name/column pair
			if (item.contains("id")) {
				dataMembers.push_back(item["id"].get<std::string>());

				//store the primaryKey column for writing sql query
				if (item.contains("column")) {
					primaryKeyColumn = item["column"].get<std::string>();
				}
				else {
					primaryKeyColumn = item["id"].get<std::string>();
				}
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

	if (subObject) {
		sqlQuery = "select * from " + tableName + " where " + "primaryKey" + "= " + "'" + primaryKeyValue.to_string() + "'";
	}
	else {
		sqlQuery = "select * from " + tableName + " where " + primaryKeyColumn + "= " + "'" + primaryKeyValue.to_string() + "'";
	}



	std::cout << sqlQuery << std::endl;



	//get the result set object and assign the values to the empty object


	//execute the sql query and retrieve the resultSet
	resultSet = statement->executeQuery(sqlQuery);

	if (resultSet->next()) {
		int count = 0;

		if (subObject == false) {

			for (std::string dataMember : dataMembers) {
				//get the type object for primary data Member which may be a class or fundamental data type

				type data_type_check = type_obj.get_property(dataMember).get_value(var_obj).get_type();

				if (!data_type_check.is_class() || data_type_check == type::get<std::string>()) {
					property prop = type_obj.get_property(dataMember);
					type propType = prop.get_type();

					//check the data type of property(data member) and execute the necessary code
					if (propType == type::get<std::string>()) {

						std::string value = resultSet->getString(columnNames[count]);
						prop.set_value(var_obj, value);

					}
					else if (propType == type::get<int>()) {

						int value = resultSet->getInt(columnNames[count]);
						prop.set_value(var_obj, value);
					}
					else if (propType == type::get<float>()) {

						float value = (float)resultSet->getDouble(columnNames[count]);
						prop.set_value(var_obj, value);

					}
					count++;
				}
				else {

					//variant for data member
					variant var_dataMember = type_obj.get_property(dataMember).get_value(var_obj);


					//getting the new table name = MainClass_SubClass
					std::string newTableName = tableName + "_" + var_dataMember.get_type().get_name().to_string();

					//getting the sub object         //data members class name                //same primary key value      

					variant var_subObject = new_get_v_2(main_object,primaryKeyValue.to_string(), true, newTableName, dataMember);

					var_obj.get_type().get_property(dataMember).set_value(var_obj, var_subObject);
				}
			}
		}


		else {

			//get the type of the main object

			for (std::string dataMember : dataMembers) {
				//get the type object for primary data Member which may be a class or fundamental data type

				type data_type_check = type_obj.get_property(dataMember).get_value(var_obj).get_type();

				if (!data_type_check.is_class() || data_type_check == type::get<std::string>()) {
					property prop = var_obj.get_type().get_property(dataMember);

					type propType = prop.get_type();

					//check the data type of property(data member) and execute the necessary code
					if (propType == type::get<std::string>()) {
						std::string value = resultSet->getString(columnNames[count]);
						prop.set_value(var_obj, value);

					}
					else if (propType == type::get<int>()) {

						int value = resultSet->getInt(columnNames[count]);
						prop.set_value(var_obj, value);
					}
					else if (propType == type::get<float>()) {

						float value = (float)resultSet->getDouble(columnNames[count]);
						prop.set_value(var_obj, value);

					}
					count++;
				}


				else {
					//variant for data member
					variant var_dataMember = type_obj.get_property(dataMember).get_value(var_obj);

					//getting the new table name = MainClass_SubClass
					std::string newTableName = type_obj.get_name().to_string() + "_" + var_dataMember.get_type().get_name().to_string();

					//getting the sub object         //data members class name                //same primary key value         
					variant var_subObject = new_get_v_2(main_object, primaryKeyValue.to_string(), true, newTableName, dataMember);
				}
			}
		}
	}
	else {
		throw std::runtime_error("no data found for the given primary key in the database table");
	}
	std::cout << var_obj.get_type().get_name() << std::endl;
	return var_obj;
}















//NEW_GET_V2


















#endif


