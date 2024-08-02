#include <string>
#include <iostream>
#include "Student.h"

std::string checkLogin(std::string, std::string);
std::string addStudent(Student s);
std::string searchStudent(int, Student&);
std::string updateStudent(Student& student);
std::string deleteStudent(int sid);

void mainKushal() {


	//login page: 


	//take the username and password from the user and call checkLogin() method
	//based on the return value of the checkLogin() method, display the output on the screen
	//checkLogin() can return three string: 1) valid_username_password 
										//  2) invalid_username
										//	3) invalid_password


	std::string username;
	std::string password;

	std::cout << "Enter Username: ";
	std::cin >> username;

	std::cout << "Enter Password: ";
	std::cin >> password;

	std::string status = checkLogin(username, password);

	if (status == "valid_username_password") {
		int option = 0; 
		std::cout << "login success" << std::endl;

		while(true) {
			std::cout << std::endl;
			std::cout << "select one of the option:: " << std::endl;
			std::cout << "1) add student" << std::endl;
			std::cout << "2) search student" << std::endl;
			std::cout << "3) update student" << std::endl;
			std::cout << "4) delete student" << std::endl;
			std::cout << std::endl << "option: ";
			std::cin >> option;


			//takes id, name, addrses, and department information from the User and creates an object to call addStudent(Student student) method;
			if (option == 1) {
				int id;
				std::string name;
				std::string addr;
				std::string department;
				std::cout << std::endl;

				std::cout << "student details: " << std::endl;

				std::cout << "enter id(must be unique): ";
				std::cin >> id;

				std::cout << "enter name: ";
				std::cin >> name;

				std::cout << "enter address: ";
				std::cin >> addr;

				std::cout << "enter department: ";
				std::cin >> department;

				Student student(id, name, addr, department);

				status = addStudent(student);
				std::cout << std::endl;
				if (status == "success") {
					std::cout << "Student Inserted Successfully" << std::endl;
				}
				else if (status == "failed") {
					std::cout << "Student Insertion Failure" << std::endl;
				}
				else if (status == "already_exists") {
					std::cout << "Student already exists" << std::endl;
				}
			}


			//take id from the user and call searchStudent(id, student) method
			//this method searches for the student data for the given id in the database and loads the data into the student object provided in the 2nd argument

			//searchStudent() returns "success" if the database table contains data for the given id else returns "failure
			else if (option == 2) {
				int id;
				std::cout << "enter the id of the student to search:  ";
				std::cin >> id;

				Student student = Student();

				status = searchStudent(id, student);

				if (status == "success") {
					std::cout << student.sid << "\t" << student.sname << "\t" << student.saddr << "\t" << student.sdepartment << std::endl;
				}
				else if (status == "failure") {
					std::cout << "no student found with sid: " << id << std::endl;
				}
			}


			else if (option == 3) {
				int id;
				std::cout << "enter the id of the student to update: ";
				std::cin >> id;
				Student student = Student();
				status = searchStudent(id, student);

				if (status == "success") {
					std::cout << student.sid << "\t" << student.sname << "\t" << student.saddr << "\t" << student.sdepartment << std::endl;


					//now take the updated values;
					std::string name;
					std::string addr;
					std::string department;
					std::cout << std::endl;

					std::cout << "update details: " << std::endl;

					std::cout << "enter name: ";
					std::cin >> name;

					std::cout << "enter address: ";
					std::cin >> addr;

					std::cout << "enter department: ";
					std::cin >> department;

					Student student(id, name, addr, department);

					std::string status = updateStudent(student);

					if (status == "success") {
						std::cout << "student updation successful" << std::endl;
					}
					else if (status == "failure") {
						std::cout << "student updation failure" << std::endl;
					}

				}
				else if (status == "failure") {
					std::cout << "no student found with sid: " << id << std::endl;
				}
			}


			else if (option == 4) {
				int id;
				std::cout << "enter the id of the student to delete: ";
				std::cin >> id;
				Student student;

				std::string status = searchStudent(id, student);


				if (status == "success") {
					status = deleteStudent(id);
					if (status == "success") {
						std::cout << "success" << std::endl;
					}
					else if (status == "failure") {
						std::cout << "failure" << std::endl;
					}
				}
				else if(status == "failure") {
					std::cout << "no student found with sid: " << id << std::endl;
				}
			}
		}
	}
	else if (status == "invalid_username") {
		std::cout << "wrong username" << std::endl;
	}
	else if (status == "invalid_password") {
		std::cout << "wrong password " << std::endl;
	}
}













