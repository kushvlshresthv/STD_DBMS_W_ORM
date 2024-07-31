#include "Configuration.h"
#include "SessionFactory.h"
#include <string>
#include "Session.h"
#include "Employee.h"
#include "Student.h"
#include "exception"


void mainKushal() {
	try {
		Configuration* cfg = new Configuration();
		cfg->configure();
		SessionFactory* sessionFactory = cfg->buildSessionFactory();
		Session* session = sessionFactory->openSession();
		Employee emp1(666, "Kushal", 9000, "Lalitpur");
		session->save(emp1);
		Student s1("222", "Shrestha", "Kathmandu");
		session->save(s1);
	}
	catch (std::exception e) {
		std::cerr << "exception:: " << e.what() << std::endl;
	}

}






