#ifndef STUDENT_H
#define STUDENT_H


#include <iostream>
#include <rttr/registration>
class Student {
public: 
	int sid;
	std::string sname;
	std::string saddr;

	Student(int no, std::string name, std::string addr)
		:sid{ no }, sname{ name }, saddr{ addr }
	{

	}
};



#endif


