#include "Configuration.h"
#include <iostream>
#include <fstream>
#include "SessionFactory.h"



void mainKushal() {
	Configuration* cfg = new Configuration();
	cfg->configure();

	cfg->buildSessionFactory();
}