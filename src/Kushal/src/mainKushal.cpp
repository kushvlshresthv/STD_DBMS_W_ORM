#include "Configuration.h"
#include <iostream>
#include <fstream>


void mainKushal() {
	Configuration* cfg = new Configuration();
	cfg->configure();

	cfg->configure("./src/Kushal/json/configuration.orm.json");

	std::ifstream file1("./src/Kushal/json/configuration.orm.json");

	cfg->configure(file1);
}