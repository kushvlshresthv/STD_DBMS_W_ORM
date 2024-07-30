#include "Configuration.h"
#include <iostream>
#include <fstream>
#include "SessionFactory.h"
#include <rttr/type>
#include <rttr/registration>
#include <string>
#include <rttr/registration_friend.h>
#include "Session.h"



void mainKushal() {
	Configuration* cfg = new Configuration();
	cfg->configure();
	SessionFactory* sessionFactory = cfg->buildSessionFactory();
	std::cout<<validateJson("./src/Kushal/STD_DBMS/json/Employee.mapping.json");
}






