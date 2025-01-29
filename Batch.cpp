#include "Command.h"
#include "Parser.h"
#include "Factory.h"
#include "ErrorHandling.h"
#include <iostream>

// to do
// ne moze da ucitava pipeline trenutno
void Batch::execute() {
	std::string input;

	ifBufferNotEmpty();
	if (argument.empty()) {
		input = ifArgumentEmpty();
	}
	else {
		input = getArgumentType();
		if (input.empty()) return;
	}

	int maxCommands = 1;
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '\n') {
			maxCommands++;
		}
	}

	std::string* commandsArray = new std::string[maxCommands];
	int commandCount = 0;

	std::string currentCommand;
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '\n') {
			if (commandCount < maxCommands) {
				commandsArray[commandCount++] = currentCommand;
				currentCommand.clear();
			}
			else {
				std::cerr << "Error: Command count exceeds allocated size." << std::endl;
				break;
			}
		}
		else {
			currentCommand += input[i];
		}
	}

	if (!currentCommand.empty() && commandCount < maxCommands) {
		commandsArray[commandCount++] = currentCommand;
	}

	for (int i = 0; i < commandCount; i++) {
		Parser commandParser;
		Factory factory;

		Parser::ParsedCommand parsedCommand = commandParser.parseCommand(commandsArray[i]);
		ErrorHandling* errorHandling = new ErrorHandling;
		if (errorHandling->catchErrors(commandsArray[i], parsedCommand) || !errorHandling->validateCommand(parsedCommand)) {
			continue;
		}
		Command* command = factory.createCommand(parsedCommand.commandName, parsedCommand.commandOpt, parsedCommand.commandArg, parsedCommand.streams);

		command->execute();
		delete command;
		delete errorHandling;
	}

	delete[] commandsArray;
}