#include "Command.h"
#include "Parser.h"
#include "CommandFactory.h"
#include <iostream>

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
		CommandFactory commandFactory;

		Parser::ParsedCommand parsedCommand = commandParser.parseCommand(commandsArray[i]);
		Command* command = commandFactory.createCommand(parsedCommand.commandName, parsedCommand.commandOpt, parsedCommand.commandArg, parsedCommand.streams);

		if (!command) {
			std::cerr << "Unknown command: \"" << parsedCommand.commandName << "\"" << std::endl;
			continue;
		}

		command->execute();
		delete command;
	}

	delete[] commandsArray;
}