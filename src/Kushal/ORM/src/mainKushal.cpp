#include "Configuration.h"
#include "SessionFactory.h"
#include <string>
#include "Session.h"
#include "Employee.h"
#include "Student.h"
#include "exception"
#include "Address.h"
#include "ShoppingItems.h"
#include "Person.h"
#include "Employee_With_Address.h"


void mainKushal() {

	Configuration* cfg = new Configuration();
	cfg->configure();
	SessionFactory* sessionFactory = cfg->buildSessionFactory();
	Session* session = sessionFactory->openSession();
	try {
		//Employee emp1(333, "Kushal", 1900, "Lalitpur");
		//session->save(emp1);
		//Student s1("222", "Shrestha", "Kathmandu");
		//session->save(s1);

		//Address a1("Bagmati", "Lalitpur", "Pulchowk");
		//session->save(a1);

		//variant var_employee = session->get("Employee", 111);

		//variant var_student = session->get("Student", "111");

		//variant var_address = session->get("Address", "Bagmati");

		//Employee* emp = var_employee.get_value<Employee*>();
		//std::cout << emp->eno << "\t" << emp->ename << "\t" << emp->esal << "\t" << emp->eaddr << std::endl;

		//Student* std = var_student.get_value<Student*>();
		//std::cout << std->sid << "\t" << std->sname << "\t" << std->saddr << std::endl;
		
		//Address* adr = var_address.get_value<Address*>();
		//std::cout << adr->state << "\t" << adr->district << "\t" << adr->city << std::endl;

		//Employee e1(444, "KuSSL", 7000, "Pokhara");
		//session->saveOrUpdate(e1);

		//Student s1("111", "KU", "Butwal");
		//session->remove(s1);

		//ShoppingItems sh1{ "Mouse", 12, 20 };
		//session->remove(sh1);
		Address adr{ "Dang", "Draks", "Patna" };
		Employee_With_Address ea1{ 333, "Kushal", adr };

		//session->save(ea1);

		//variant op = session->new_get("Employee_With_Address", 111, Employee_With_Address());
		//Employee_With_Address e = op.get_value<Employee_With_Address>();
		//std::cout << e.ename;
		//std::cout << e.eaddr.state << std::endl;


		variant op1 = session->new_get_v_2(Employee_With_Address(), 333);
		Employee_With_Address e1 = op1.get_value<Employee_With_Address>();
		std::cout << e1.eno << std::endl;
		std::cout << e1.ename << std::endl;
		std::cout << e1.eaddr.state << std::endl;
		std::cout << e1.eaddr.city << std::endl;
		std::cout << e1.eaddr.district << std::endl;
		std::cout << "Works Fine";

		session->commit();
	}
	catch (std::exception e) {
		std::cerr << "exception:: " << e.what() << std::endl;
		session->rollback();
	}

}


/*#include <rttr/registration>
#include <rttr/type>
#include <rttr/variant.h>

void mainKushal() {
	type  obj = type::get_by_name("Employee");

	if (obj.is_valid()) {
		variant var = obj.create();
		property prop = obj.get_property("eno");
		prop.set_value(var, 111);


		Employee* e = var.get_value<Employee*>();
		std::cout << "Employee object created succesfully" << std::endl;
		std::cout << e->eno << std::endl;
	}
}*/




/*void mainKushal() {
	Address address{ "bagmati", "lalitpur", "patan" };
	Person person{ "kushal", address };
	Employee e1{ 111, "Kushal", 9000, "Lalitpur" };
	trial::save(person);
	trial::save(e1);
	Student s1{ "111", "Hi", "Hello" };
	trial::save(s1);
}*/

/*void mainKushal() {
	Address address{ "bagmati", "lalitpur", "patan" };
	Person person{ "kushal", address };

	variant var_address = address;
	variant var_person = person;

	type type_address = var_address.get_type();
	type type_person = var_person.get_type();

	Address newAddress{ "new bagmati", "new lalitpur", "new patan" };
	variant var_newAddress = newAddress;

	property prop_address = type_person.get_property("address");
	prop_address.set_value(var_person, newAddress);
	// Retrieve the value as a Person object
	Person personWithNewAddress = var_person.get_value<Person>();

	std::cout << "Hello" << std::endl;
	std::cout << personWithNewAddress.address.state << std::endl;
}*/

void mainKushal1() {
	Student s1("111", "222", "333");
	variant var_s1 = s1;

	std::cout << &s1 << std::endl;
	std::cout << &var_s1 << std::endl;

	var_s1.get_type().get_property("sname").set_value(var_s1, "999");
	std::cout << s1.sname;
}









