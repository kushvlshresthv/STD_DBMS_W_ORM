#include <string>
#include "Configuration.h"
#include "SessionFactory.h"
#include "Session.h"
#include "Login_Details.h"
#include "Student.h"

Configuration* cfg = new Configuration();
SessionFactory* sessionFactory;
Session* session;

std::string checkLogin(std::string username, std::string password) {
	cfg->configure();
	sessionFactory = cfg->buildSessionFactory();
	session = sessionFactory->openSession();
	try {
		Login_Details login_details = session->get(Login_Details(), username).get_value<Login_Details>();
		if (login_details.password == password) {
			return "valid_username_password";
		}
		else {
			return "invalid_password";
		}
	}
	catch (std::exception& e) {
		return "invalid_username";

	}
}


//retruns "success" or "already_exists" or "failure";

std::string addStudent(Student student) {
	try {
		cfg->configure();
		sessionFactory = cfg->buildSessionFactory();
		session = sessionFactory->openSession();
		session->save(student);
		session->commit();
		return "success";
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() == 1062) {  // MySQL error code for duplicate entry
			return "already_exists";
		}
	}
	catch (std::exception) {
		return "failure";
	}
}


//search the student based on the primary key
//for student object, sid is the primary key which is unique for all student


std::string searchStudent(int sid, Student& student) {
	try {
		cfg->configure();
		sessionFactory = cfg->buildSessionFactory();
		session = sessionFactory->openSession();
		student = session->get(Student(),sid).get_value<Student>();
		return "success";
	}
	catch (std::exception& e) {
		return "failure";
	}
}


std::string updateStudent(Student& student) {
	try {
		cfg->configure();
		sessionFactory = cfg->buildSessionFactory();
		session = sessionFactory->openSession();
		Student std = Student();
		session->update(student);
		session->commit();
		return "success";
	}
	catch (std::exception& e) {
		return "failure";
	}
}

std::string deleteStudent(int sid) {
	try {
		cfg->configure();
		sessionFactory = cfg->buildSessionFactory();
		session = sessionFactory->openSession();
		Student student(sid, "", "", "");
		session->remove(student);
		return "success";
	}
	catch (std::exception& e) {
		return "failure";
	}
}