#ifndef EMPLOYEE_H
#define EMPLOYEE_H


#include <string>
#include <rttr/registration>

class Employee {
public: 
	int eno;
	std::string ename;
	float esal;
	std::string eaddr;

	Employee(int no, std::string name, float sal, std::string addr)
		: eno{ no },
		ename{ name }, 
		esal{ sal }, 
		eaddr{ addr } 
	{

	}

	Employee() = default;
};




#endif