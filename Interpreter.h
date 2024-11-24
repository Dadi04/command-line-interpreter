#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Command.h"
#include "CommandParser.h"
#include "CommandFactory.h"

const int MAX_INPUT_LENGHT = 512;

class Interpreter {
private:
	CommandFactory commandFactory;
	CommandParser commandParser;
	std::string prompt = "$ ";

public:
	void run() {
		
		char input[MAX_INPUT_LENGHT];

		while (true) {
			std::cout << prompt;
			std::cin.getline(input, MAX_INPUT_LENGHT);

			if (input[0] == '\0') continue;

			// moguce greske prilikom implementiranje logike za '<' '>>' '>' i '|' i oko logike za izvrsavanje vise komandi odjednom
			std::string* parsedInput = commandParser.parse(input);
			if (parsedInput != nullptr) {
				std::string commandName = parsedInput[0];
				std::string opt = (parsedInput[1].empty()) ? "" : parsedInput[1];
				std::string arg = (parsedInput[2].empty()) ? "" : parsedInput[2];

				auto command = commandFactory.createCommand(commandName, opt, arg);
				delete[] parsedInput;

				if (command) {
					command->execute();
					delete command;
				}
				else {
					std::cerr << "Nepoznata komanda: " << commandName << std::endl;
				}
			}
			else {
				std::cerr << "Greska prilikom parsiranja." << std::endl;
			}
		}
	}
};

#endif