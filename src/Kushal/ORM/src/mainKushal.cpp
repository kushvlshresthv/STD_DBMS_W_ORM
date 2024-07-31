#include "Configuration.h"
#include "SessionFactory.h"
#include <string>
#include "Session.h"
#include "Employee.h"
#include "Student.h"
#include "exception"


void mainKushal() {

	Configuration* cfg = new Configuration();
	cfg->configure();
	SessionFactory* sessionFactory = cfg->buildSessionFactory();
	Session* session = sessionFactory->openSession();
	try {
		Employee emp1(777, "Kushal", 9000, "Lalitpur");
		session->save(emp1);
		Student s1("333", "Shrestha", "Kathmandu");
		session->save(s1);

		session->commit();
	}
	catch (std::exception e) {
		std::cerr << "exception:: " << e.what() << std::endl;
		session->rollback();
	}

}






