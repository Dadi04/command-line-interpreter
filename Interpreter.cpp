#include "Interpreter.h"
#include <iostream>

const int MAX_INPUT_LENGTH = 512;

void Interpreter::run() {
	Parser commandParser;
	CommandFactory commandFactory;
	char input[MAX_INPUT_LENGTH];

	while (true) {
		std::cout << Prompt::getPromptSign();
		std::cin.getline(input, MAX_INPUT_LENGTH);

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Input exceeds the maximum length of " << MAX_INPUT_LENGTH << " characters." << std::endl;
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
