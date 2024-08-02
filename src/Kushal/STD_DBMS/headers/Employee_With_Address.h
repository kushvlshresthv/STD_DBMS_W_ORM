#ifndef EMP_W_ADDRESS_H
#define EMP_W_ADDRESS_H

#include <iostream>
#include "Address.h"

class Employee_With_Address {
public: 
	int eno;
	std::string ename;
	Address eaddr;

	Employee_With_Address(int no, std::string name, Address addr)
		:eno{ no }, ename{ name }, eaddr{ addr } {

	};

	Employee_With_Address() {
		std::cout << "Employee_with_address constructor executed" << std::endl;
	}
};


#endif