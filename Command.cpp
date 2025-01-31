#include "Command.h"
#include <iostream>
#include <fstream>

// metoda koja odredjuje da li je argument string ili file
std::string Command::getArgumentType() {
	std::string input;
	if (!argument.empty()) {
		if (argument.front() == '"' && argument.back() == '"') {
			input = argument.substr(1, argument.size() - 2);
		}
		else {
			std::ifstream file(argument);
			if (file.is_open()) {
				std::string line;
				while (std::getline(file, line)) {
					input += line + '\n';
				}
				if (!input.empty() && input.back() == '\n') {
					input.pop_back();
				}
				file.close();
			}
			else {
				std::cerr << "Error: File \"" << argument << "\" does not exist." << std::endl;
				return "";
			}
		}
	}
	return input;
}

std::string Command::ifArgumentEmpty() {
	if (argument.empty()) {
		std::string line;
		while (true) {
			if (!std::getline(std::cin, line)) break;
			argument += line + '\n';
		}
	}

	return argument;
}

std::string Command::ifBufferNotEmpty() {
	if (buffer && !buffer->str().empty()) {
		argument = buffer->str();
	}

	return argument;
}

void Command::setBuffer(std::stringstream* buffer) {
	this->buffer = buffer;
}

std::string Command::getBuffer() {
	return this->buffer->str();
}

std::string Command::getName() {
	return this->name;
}

std::vector<Redirection> Command::getStreams() {
	return this->streams;
}

bool Command::RedirectInput(std::string& input) {
	for (Redirection& stream : streams) {
		if (stream.redirectInput(input)) {
			return true;
		}
	}
	return false;
}

bool Command::RedirectOutput(std::string input) {
	for (Redirection& stream : streams) {
		if (stream.redirectOutput(input)) {
			return true;
		}
	}
	return false;
}
