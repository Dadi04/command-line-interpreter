#include "Parser.h"
#include <iostream>

Parser::ParsedCommand Parser::parseCommand(std::string input) {
	ParsedCommand parsedCommand;
	int i = 0;
	
	skipWhiteSpace(input, i);
	
	parsedCommand.commandName = readToken(input, i);

	skipWhiteSpace(input, i);

	if (i < input.length() && input[i] == '-') {
		parsedCommand.commandOpt = readToken(input, i);
	}

	skipWhiteSpace(input, i);

	if (i < input.length()) {
		parsedCommand.commandArg = readArgument(input, i);
	}

	return parsedCommand;
}

void Parser::skipWhiteSpace(std::string input, int& i) {
	while (i < input.length() && (std::isspace(input[i]) || input[i] == '\t')) {
		i++;
	}
}

std::string Parser::readToken(std::string input, int& i) {
	std::string token;
	while (i < input.length() && !std::isspace(input[i]) && input[i] != '\t') {
		token += input[i];
		i++;
	}
	return token;
}

std::string Parser::readArgument(std::string input, int& i) {
	bool inQuotes = false;
	std::string token;

	if (input[i] == '"') {
		token += input[i];
		inQuotes = true;
		i++;
	}
	while (i < input.length()) {
		if (input[i] == '"') {
			token += input[i];
			inQuotes = !inQuotes;
			i++;
			if (inQuotes) break;
		}
		else {
			token += input[i];
			i++;
		}
	}
	return token;
}

// echo "sajw"qwdas" izbacuje sajw
// naci poslednji tj zatvoreni navodnik i od toga naci argument