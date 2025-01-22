#include "Command.h"
#include <iostream>
#include <fstream>

void Touch::execute() {
	ifBufferNotEmpty();

	std::ifstream file(argument);
	if (file) {
		std::cerr << "Error: File \"" << argument << "\" already exists." << std::endl;
	}
	else {
		std::ofstream newFile(argument);
		if (newFile) {
			newFile.close();
			std::cout << "File \"" << argument << "\" created successfully." << std::endl;
		}
		else {
			std::cerr << "Error: File \"" << argument << "\" could not be created." << std::endl;
		}
	}
}

void Touch::print() {

}
