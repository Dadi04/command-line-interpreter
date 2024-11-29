#include "Interpreter.h"
#include <iostream>

const int MAX_INPUT_LENGHT = 512;

void Interpreter::run() {
	Parser commandParser;
	CommandFactory commandFactory;
	char input[MAX_INPUT_LENGHT];

	while (true) {
		std::cout << prompt;
		std::cin.getline(input, MAX_INPUT_LENGHT);

		// ????
		// trenutno kada ubacim 512 karakter ode sve u k i nastane infinite loop
		if (std::cin.gcount() == MAX_INPUT_LENGHT - 1 && input[MAX_INPUT_LENGHT - 2] != '\0') {
			std::cout << "Input exceeds the maximum lenght of " << MAX_INPUT_LENGHT << " characters" << std::endl;
			continue;
		}

		if (input[0] == '\0') continue;

		Parser::ParsedCommand parsedCommand = commandParser.parseCommand(input);
		Command* command = commandFactory.createCommand(parsedCommand.commandName, parsedCommand.commandOpt, parsedCommand.commandArg);

		if (command == nullptr) {
			std::cout << "Unknown command: \"" << parsedCommand.commandName << "\"" << std::endl;
			continue;
		}

		command->execute();

		delete command;

		if (std::cin.eof()) {
			std::cin.clear();
			continue;
		}
	}
}
