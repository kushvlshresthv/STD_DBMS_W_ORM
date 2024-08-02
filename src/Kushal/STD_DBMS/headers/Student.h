#ifndef STUDENT_H
#define STUDENT_H

#include <string>


class Student {
public: 
	int sid;
	std::string sname;
	std::string saddr;
	std::string sdepartment;

	Student(int id, std::string name, std::string addr, std::string department)
		:sid{ id }, sname{ name }, saddr{ addr }, sdepartment{ department }
	{

	}


	Student() = default;
};

#endif