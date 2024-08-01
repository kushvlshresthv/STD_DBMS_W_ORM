#include "Configuration.h"
#include "SessionFactory.h"
#include <string>
#include "Session.h"
#include "Employee.h"
#include "Student.h"
#include "exception"
#include "Address.h"
#include "ShoppingItems.h"



void mainKushal() {

	Configuration* cfg = new Configuration();
	cfg->configure();
	SessionFactory* sessionFactory = cfg->buildSessionFactory();
	Session* session = sessionFactory->openSession();
	try {
		//Employee emp1(222, "Kushal", 9000, "Lalitpur");
		//session->save(emp1);

		//Student s1("222", "Shrestha", "Kathmandu");
		//session->save(s1);

		//Address a1("Bagmati", "Lalitpur", "Pulchowk");
		//session->save(a1);

		//session->commit();

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

		Student s1("111", "KU", "Butwal");
		session->remove(s1);

		ShoppingItems sh1{ "Mouse", 12, 20 };
		session->remove(sh1);



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






