#include "Student.h"
#include "Employee.h"

RTTR_REGISTRATION{
	rttr::registration::class_<Student>("Student").property("sid",&Student::sid).property("sname", &Student::sname).property("saddr",&Student::saddr);
	rttr::registration::class_<Employee>("Employee").property("eno", &Employee::eno).property("ename", &Employee::ename).property("eaddr", &Employee::eaddr).property("esal", &Employee::esal);
}