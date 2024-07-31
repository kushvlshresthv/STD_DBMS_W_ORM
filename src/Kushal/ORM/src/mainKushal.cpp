#include "Configuration.h"
#include "SessionFactory.h"
#include <string>
#include "Session.h"
#include "Employee.h"
#include "Student.h"
#include "exception"
#include "Address.h"



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

		variant var = session->get("Employee", 111);

		Employee* emp = var.get_value<Employee*>();

		std::cout << emp->eno << std::endl;

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






