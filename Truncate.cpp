#include "Command.h"
#include <iostream>
#include <fstream>

void Truncate::execute() {
	ifBufferNotEmpty();

	RedirectInput(argument);

	if (argument.front() == '"' && argument.back() == '"') {
		argument = argument.substr(1, argument.size() - 2);
	}

	std::ifstream file(argument);
	if (!file) {
		std::cerr << "Error: File \"" << argument << "\" does not exist." << std::endl;
		file.close();
	}
	else {
		std::ofstream file(argument, std::ios::trunc);
		if (file.is_open()) {
			std::cout << "Content in file \"" << argument << "\" deleted successfully." << std::endl;
			file.close();
		}
		else {
			std::cerr << "Error: Failed to open \"" << argument << "\"." << std::endl;
		}
	}
}