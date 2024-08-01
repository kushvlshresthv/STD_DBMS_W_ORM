#include "Student.h"
#include "Employee.h"
#include "Address.h"
#include <ShoppingItems.h>
#include "Person.h"
#include "Employee_With_Address.h"
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

}