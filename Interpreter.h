#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Command.h"
#include "Parser.h"
#include "CommandFactory.h"
#include "ErrorHandling.h"

const int MAX_INPUT_LENGHT = 512;

class Interpreter {
private:
	Parser commandParser;
	std::string prompt = "$ ";

public:
	void run() {
		
		char input[MAX_INPUT_LENGHT];

		while (true) {
			std::cout << prompt;
			std::cin.getline(input, MAX_INPUT_LENGHT);

			if (input[0] == '\0') continue;

			Command* command = commandParser.parseCommand(input);
			if (command == nullptr) {
				std::cout << "Nepoznata komanda." << std::endl;
				continue;
			}
			command->execute();

			delete command;
			
		}
	}
};

#endif