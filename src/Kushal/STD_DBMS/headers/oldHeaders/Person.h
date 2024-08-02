#ifndef PERSON_H
#define PERSON_H

#include "Address.h"
#include <string>

class Person {
public: 
	std::string name;
	Address address;

	Person(std::string p_name, Address p_address) 
		: name{p_name}, address{p_address}
	{

	}

	Person() = default;
};


#endif