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

		if (input[0] == '\0') continue;
		// provera da li je preko 512 karaktera

		Parser::ParsedCommand parsedCommand = commandParser.parseCommand(input);
		Command* command = commandFactory.createCommand(parsedCommand.commandName, parsedCommand.commandOpt, parsedCommand.commandArg);

		if (command == nullptr) {
			std::cout << "Unknown command: \"" << parsedCommand.commandName << "\"" << std::endl;
			continue;
		}

		command->execute();

		delete command;
	}
}
