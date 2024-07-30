#include "Configuration.h"
#include <iostream>

void mainKushal() {
	Configuration* cfg = new Configuration();
	if (cfg->validateJson("./src/Kushal/json/configuration.orm.json")) {
		std::cout << "success";
	}
	else {
		std::cout << "failure";
	}
}