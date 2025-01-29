#include "Command.h"
#include <iostream>
#include <fstream>

void Rm::execute() {
	ifBufferNotEmpty();

	if (!RedirectInput(argument)) {
		return;
	}

	std::ifstream file(argument);
	if (!file) {
		std::cerr << "Error: File \"" << argument << "\" does not exist." << std::endl;
		file.close();
	}
	else {
		file.close();
		int status = remove(argument.c_str());
		if (status == 0) {
			std::cout << "File \"" << argument << "\" deleted successfully." << std::endl;
		}
		else {
			std::cerr << "Error: Failed to delete \"" << argument << "\"." << std::endl;
		}
	}
}