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
		buffer->str("");
		buffer->clear();
	}

	return argument;
}

void Command::setBuffer(std::stringstream* buffer) {
	this->buffer = buffer;
}

void Command::RedirectInput(std::string& input) {
	for (Redirection& stream : streams) {
		if (stream.redirectInput(input)) {
			return;
		}
	}
}

void Command::RedirectOutput(std::string input) {
	bool redirected = false;
	for (Redirection& stream : streams) {
		if (stream.redirectOutput(input)) {
			redirected = true;
			return;
		}
	}

	if (!redirected) {
		if (buffer && buffer->str().empty()) {
			*buffer << "\"" + input + "\"";
		}
		else {
			std::cout << input << std::endl;
		}
	}
}

//void Command::pipelineRedirectOutput() {
//	for (Redirection& stream : streams) {
//		if (stream.type != Redirection::Output && stream.type != Redirection::Append) {
//			std::string output = buffer->str();
//			if (!output.empty()) {
//				if (output.front() == '"' && output.back() == '"') {
//					output = output.substr(1, output.size() - 2);
//				}
//				buffer->str("");
//				buffer->clear();
//				std::cout << output << std::endl;
//				return;
//			}
//		}
//	}
//}
