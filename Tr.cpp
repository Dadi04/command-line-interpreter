#include "Command.h"
#include <iostream>
#include <string>

// nema ulaznu redirekciju tj ne prihvata znak <, ukoliko dodje znak < program zabode
void Tr::execute() {
	ifBufferNotEmpty();

	if (argument.empty()) {
		std::cerr << "Error: Command tr must have an argument" << std::endl;
		return;
	}
	if (what.empty()) {
		std::cerr << "Error: Command tr must have a 'what' you want to replace with" << std::endl;
		return;
	}

	std::string input = getArgumentType();

	if (input.empty()) return;

	size_t counter = 0;
	while ((counter = input.find(what, counter)) != std::string::npos) {
		input.replace(counter, what.length(), with);
		counter += with.length();
	}

	RedirectOutput(input);
}

void Tr::print() {

}

void Tr::parseArguments(std::string arg) {
	std::string argument, what, with;
	int i = 0;
	if (arg.empty()) {
		return;
	}

	if (i < arg.length() && arg[i] == '"') {
		i++;
		while (i < arg.length()) {
			if (arg[i] == '"') {
				i++;
				break;
			}
			argument += arg[i];
			i++;
		}
		if (!argument.empty()) {
			this->argument = '\"' + argument + '\"';
		}
		else {
			this->argument = "";
			return;
		}
	}
	else if (i < arg.length() && arg[i] != '<' && arg[i] != '>') {
		while (i < arg.length()) {
			if (std::isspace(arg[i])) {
				break;
			}
			argument += arg[i];
			i++;
		}
		this->argument = argument;
	}

	while (i < arg.length() && (std::isspace(arg[i]) || arg[i] == '\t')) {
		i++;
	}

	if (i < arg.length() && arg[i] == '"') {
		i++;
		while (i < arg.length()) {
			if (arg[i] == '"') {
				i++;
				break;
			}
			what += arg[i];
			i++;
		}
		this->what = what;
	}
	else {
		return;
	}

	while (i < arg.length() && (std::isspace(arg[i]) || arg[i] == '\t')) {
		i++;
	}

	if (i < arg.length() && arg[i] == '"') {
		i++;
		while (i < arg.length()) {
			if (arg[i] == '"') {
				break;
			}
			with += arg[i];
			i++;
		}
		this->with = with;
	}
}