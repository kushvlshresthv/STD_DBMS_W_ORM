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
		Employee emp1(111, "Kushal", 9000, "Lalitpur");
		session->save(emp1);

		Student s1("111", "Shrestha", "Kathmandu");
		session->save(s1);

		Address a1("Bagmati", "Lalitpur", "Patan");
		session->save(a1);

		session->commit();
	}
	catch (std::exception e) {
		std::cerr << "exception:: " << e.what() << std::endl;
		session->rollback();
	}

}






