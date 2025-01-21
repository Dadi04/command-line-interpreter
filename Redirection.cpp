#include "Redirection.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

bool Redirection::redirectInput(std::string& input) {
	if (type == Input) {
		std::ifstream inputFile;
		inputFile.open(file);
		if (!inputFile.is_open()) {
			std::cerr << "Error: File \"" << file << "\" does not exist." << std::endl;
			return false;
		}

		std::stringstream buffer;
		buffer << inputFile.rdbuf();
		input = buffer.str();
		inputFile.close();
		return true;
	}
	return false;
}

bool Redirection::redirectOutput(std::string output) {
	if (type == Output || type == Append) {
		std::ofstream outputFile;
		if (type == Output) {
			outputFile.open(file, std::ios::trunc);
		}
		else if (type == Append) {
			outputFile.open(file, std::ios::app);
		}

		if (!outputFile.is_open()) {
			std::cerr << "Error: File \"" << file << "\" does not exist." << std::endl;
			return false;
		}
		outputFile << output;
		outputFile.close();
		return true;
	}
	return false;
}
