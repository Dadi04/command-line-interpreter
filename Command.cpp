#include "Command.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

std::string Command::getArgumentInput() {
	std::string input;
	if (argument.front() == '"' && argument.back() == '"') {
		input = argument.substr(1, argument.size() - 2);
	}
	else {
		std::ifstream file(argument);
		if (file.is_open()) {
			std::ostringstream ss;
			ss << file.rdbuf();
			input = ss.str();
			file.close();
		}
		else {
			std::cerr << "Error: File \"" << argument << "\" could not be opened.\n";
		}
	}
	return input;
}

void Echo::execute() {
	std::string input = getArgumentInput();
	std::cout << input << std::endl;
}

void Time::execute() {
	time_t timestamp;
	time(&timestamp);
	struct tm currentTime;

	if (localtime_s(&currentTime, &timestamp) == 0) {
		char timeString[9];
		strftime(timeString, sizeof(timeString), "%H:%M:%S", &currentTime);

		std::cout << timeString << std::endl;
	}
	else {
		std::cerr << "Error getting local time." << std::endl;
	}
}

void Date::execute() {
	time_t timestamp;
	time(&timestamp);
	struct tm currentDate;

	if (localtime_s(&currentDate, &timestamp) == 0) {
		char dateString[11];
		strftime(dateString, sizeof(dateString), "%Y-%m-%d", &currentDate);

		std::cout << dateString << std::endl;
	}
	else {
		std::cerr << "Error getting local time." << std::endl;
	}

}

void Touch::execute() {
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
			std::cerr << "Error creating file \"" << argument << "\"." << std::endl;
		}
	}
}

void Wc::execute() {
	std::string input = getArgumentInput();

	if (option == "-w") {
		int wordCount = 0;
		for (int i = 0; i < input.length(); i++) {
			if (std::isspace(input[i])) {
				wordCount++;
			}
		}
		wordCount++;

		std::cout << wordCount << std::endl;
	}
	else if (option == "-c") {
		std::cout << input.length() << std::endl;
	}
	else {
		std::cerr << "Unknown option: " << option << std::endl;
	}
}
