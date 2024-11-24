#include "Interpreter.h"
#include "Parser.h"
#include <iostream>

void Interpreter::run() {
	Parser commandParser;
	char input[MAX_INPUT_LENGHT];

	while (true) {
		std::cout << prompt;
		std::cin.getline(input, MAX_INPUT_LENGHT);

		if (input[0] == '\0') continue;

		Command* command = commandParser.parseCommand(input);
		if (command == nullptr) continue;

		command->execute();

		delete command;

	}
}