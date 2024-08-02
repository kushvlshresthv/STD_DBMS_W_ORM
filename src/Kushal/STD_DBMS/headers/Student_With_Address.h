#ifndef STUDENT_WITH_ADDRESS 
#define STUDENT_WITH_ADDRESS

#include <string>
#include <Address.h>



class Student_With_Address {
public: 
	int sid;
	Address saddr;
	Student_With_Address(int id, Address addr)
		:sid{ id }, saddr{ addr }
	{

	}

	Student_With_Address() = default;
};




#endif
