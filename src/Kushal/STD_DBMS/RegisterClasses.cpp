#include <Login_Details.h>
#include <rttr/registration>
#include "Student.h"


using namespace rttr;

RTTR_REGISTRATION{
	rttr::registration::class_<Login_Details>("Login_Details").constructor<>().property("username", &Login_Details::username).property("password", &Login_Details::password);

	rttr::registration::class_<Student>("Student").constructor<>().property("sid", &Student::sid).property("sname", &Student::sname).property("saddr", &Student::saddr).property("sdepartment", &Student::sdepartment);

}


























//these files have been moved to oldHeaders folder inside Kushal/STD_DBMS/headers

//json mapping files for these have been moved to Kushal/STD_DBMS/json/oldJsons



/* #include "Student.h"
#include "Employee.h"
#include "Address.h"
#include <ShoppingItems.h>
#include "Person.h"
#include "Employee_With_Address.h"
#include "Student_With_Address.h"
RTTR_REGISTRATION{

	//registering the Student class
	rttr::registration::class_<Student>("Student").constructor<>().property("sid",&Student::sid).property("sname", &Student::sname).property("saddr",&Student::saddr);

	//registering the Employee Class
	rttr::registration::class_<Employee>("Employee").constructor<>().property("eno", &Employee::eno).property("ename", &Employee::ename).property("eaddr", &Employee::eaddr).property("esal", &Employee::esal);

	//registering the Address Class
	rttr::registration::class_<Address>("Address").constructor<>().property("state", &Address::state).property("district", &Address::district).property("city", &Address::city);

	//registering the ShoppingItems
	rttr::registration::class_<ShoppingItems>("ShoppingItems").constructor<>().property("itemNames", &ShoppingItems::itemNames).property("quantity", &ShoppingItems::quantity).property("price", &ShoppingItems::price);


	//registering the Person Class

	rttr::registration::class_<Person>("Person").constructor<>().property("name", &Person::name).property("address", &Person::address);


	//registering Employee_With_Address

	rttr::registration::class_<Employee_With_Address>("Employee_With_Address").constructor<>().property("eno", &Employee_With_Address::eno).property("ename", &Employee_With_Address::ename).property("eaddr", &Employee_With_Address::eaddr);


	//registering Student_With_Address

	rttr::registration::class_<Student_With_Address>("Student_With_Address").constructor<>().property("sid", &Student_With_Address::sid).property("saddr", &Student_With_Address::saddr);

}
*/