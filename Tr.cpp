#include "Command.h"
#include <iostream>
#include <string>

// nema ulaznu redirekciju tj ne prihvata znak < ukoliko je on posle what i with, ako dodje na mestu de je argument onda je ok
// ukoliko nema argument trebalo bi da otvori da se pise u vise redova kao kod komande echo ili head kad nemaju argument
void Tr::execute() {
	checkBuffer();

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

std::string Tr::getWhat() {
	return this->what;
}

std::string Tr::getWith() {
	return this->with;
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

void Tr::checkBuffer() {
	if (buffer) {
		if (!argument.empty() && argument.front() == '"' && argument.back() == '"') {
			argument = argument.substr(1, argument.size() - 2);
		}
		this->with = what;
		this->what = argument;
		this->argument = ifBufferNotEmpty();
	}
}