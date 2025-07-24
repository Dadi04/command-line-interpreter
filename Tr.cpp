#include "Command.h"
#include <iostream>
#include <string>

void Tr::execute() {
	checkBuffer();

	if (what.empty()) {
		std::cerr << "Error: Command tr must have a 'what' you want to replace with" << std::endl;
		return;
	}

	std::string input;
	
	if (buffer && !buffer->str().empty()) {
		input = buffer->str();
		if (!input.empty() && input.front() == '"' && input.back() == '"') {
			input = input.substr(1, input.size() - 2);
		}
	}
	else {
		if (argument.empty()) {
			input = ifArgumentEmpty();
		}
		else {
			input = getArgumentType();
			if (input.empty()) return;
		}
	}

	size_t counter = 0;
	while ((counter = input.find(what, counter)) != std::string::npos) {
		input.replace(counter, what.length(), with);
		counter += with.length();
	}

	if (input.back() == '\n') {
		input.pop_back();
	}

	print(input);
}

void Tr::print(std::string output) {
	if (!RedirectOutput(output)) {
		if (buffer) {
			buffer->str("");
			buffer->clear();
			*buffer << "\"" + output + "\"";
		}
		else {
			std::cout << output << std::endl;
		}
	}
}

void Tr::parseArguments(std::string arg) {
	if (arg.empty()) {
		return;
	}

	std::vector<std::string> tokens;
	std::vector<bool> wasQuoted;
	int i = 0;
	
	while (i < arg.length()) {
		while (i < arg.length() && (std::isspace(arg[i]) || arg[i] == '\t')) {
			i++;
		}
		
		if (i >= arg.length()) break;
		
		std::string token;
		bool isQuoted = false;
		
		if (arg[i] == '"') {
			isQuoted = true;
			i++;
			while (i < arg.length() && arg[i] != '"') {
				token += arg[i];
				i++;
			}
			if (i < arg.length()) i++;
		}
		else {
			while (i < arg.length() && !std::isspace(arg[i]) && arg[i] != '"') {
				token += arg[i];
				i++;
			}
		}
		
		if (!token.empty()) {
			tokens.push_back(token);
			wasQuoted.push_back(isQuoted);
		}
	}

	if (tokens.size() == 1) {
		this->what = tokens[0];
		this->with = "";
		this->argument = "";
	}
	else if (tokens.size() == 2) {
		this->what = tokens[0];
		this->with = tokens[1];
		this->argument = "";
	}
	else if (tokens.size() == 3) {
		if (wasQuoted[0]) {
			this->argument = '"' + tokens[0] + '"';
		}
		else {
			this->argument = tokens[0];
		}
		this->what = tokens[1];
		this->with = tokens[2];
	}
	else if (tokens.size() >= 4) {
		this->argument = tokens[0];
		this->what = tokens[1];
		this->with = tokens[2];
	}
}

void Tr::checkBuffer() {
	if (buffer && !buffer->str().empty()) {
		this->argument = buffer->str();
	}
}