#include "Parser.h"
#include "CommandFactory.h"
#include <iostream>

Command* Parser::parseCommand(const std::string& input) {
	CommandFactory commandFactory;

	std::string commandName; std::string commandOpt; std::string commandArg;

	int i = 0;
	bool inQuotes = false;

	// Preskace beline na pocetku
	while (i < input.length() && (std::isspace(input[i]) || input[i] == '\t')) {
		i++;
	}
	// Pronalazi ime komande
	while (i < input.length() && !std::isspace(input[i]) && input[i] != '\t') {
		commandName += input[i];
		i++;
	}
	// preskace beline izmedju komande i opcije
	while (i < input.length() && (std::isspace(input[i]) || input[i] == '\t')) {
		i++;
	}
	// pronalazi opciju ako postoji
	if (i < input.length() && input[i] == '-') {
		while (i < input.length() && !std::isspace(input[i]) && input[i] != '\t') {
			commandOpt += input[i];
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
			commandArg += input[i];
			inQuotes = true;
			i++;
		}
		while (i < input.length() && (inQuotes || (!std::isspace(input[i]) && input[i] != '\t'))) {
			if (input[i] == '"') {
				commandArg += input[i];
				inQuotes = false;
				i++;
				break;
			}
			commandArg += input[i];
			i++;
		}
	}

	Command* command = commandFactory.createCommand(commandName, commandOpt, commandArg);
	if (command == nullptr) {
		std::cout << "Unknown command: \"" << commandName << "\"" << std::endl;
	}

	return command;
}