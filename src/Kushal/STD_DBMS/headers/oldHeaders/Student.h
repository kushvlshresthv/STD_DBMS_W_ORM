#ifndef STUDENT_H
#define STUDENT_H


#include <iostream>
#include <rttr/registration>
class Student {
public: 
	std::string sid;
	std::string sname;
	std::string saddr;

	Student(std::string no, std::string name, std::string addr)
		:sid{ no }, sname{ name }, saddr{ addr }
	{

	}

	Student() = default;
};



#endif


