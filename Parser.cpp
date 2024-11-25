#include "Parser.h"
#include <iostream>

Parser::ParsedCommand Parser::parseCommand(std::string input) {
	ParsedCommand parsedCommand;

	int i = 0;
	bool inQuotes = false;

	// Preskace beline na pocetku
	while (i < input.length() && (std::isspace(input[i]) || input[i] == '\t')) {
		i++;
	}
	// Pronalazi ime komande
	while (i < input.length() && !std::isspace(input[i]) && input[i] != '\t') {
		parsedCommand.commandName += input[i];
		i++;
	}
	// preskace beline izmedju komande i opcije
	while (i < input.length() && (std::isspace(input[i]) || input[i] == '\t')) {
		i++;
	}
	// pronalazi opciju ako postoji
	if (i < input.length() && input[i] == '-') {
		while (i < input.length() && !std::isspace(input[i]) && input[i] != '\t') {
			parsedCommand.commandOpt += input[i];
			i++;
		}
	}
	// preskace beline izmedju opcije i argumenta
	while (i < input.length() && (std::isspace(input[i]) || input[i] == '\t')) {
		i++;
	}
	// pronalazi argument ako postoji
	if (i < input.length()) {
		if (input[i] == '"') {
			parsedCommand.commandArg += input[i];
			inQuotes = true;
			i++;
		}
		while (i < input.length() && (inQuotes || (!std::isspace(input[i]) && input[i] != '\t'))) {
			if (input[i] == '"') {
				parsedCommand.commandArg += input[i];
				inQuotes = false;
				i++;
				break;
			}
			parsedCommand.commandArg += input[i];
			i++;
		}
	}

	return parsedCommand;
}