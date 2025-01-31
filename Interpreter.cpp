#include "Parser.h"
#include "Command.h"
#include "Factory.h"
#include "Pipeline.h"
#include "ErrorHandling.h"
#include <string>
#include <iostream>

const int MAX_INPUT_LENGTH = 512;

int main() {
	Parser commandParser;
	Factory factory;
	ErrorHandling* errorHandling = new ErrorHandling;
	char input[MAX_INPUT_LENGTH];

	while (true) {
		std::cout << Prompt::getPromptSign();
		std::cin.getline(input, MAX_INPUT_LENGTH);

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cerr << "Input exceeds the maximum length of " << MAX_INPUT_LENGTH << " characters." << std::endl;
		}

		if (input[0] == '\0') continue;

		bool insideQuotes = false, hasPipe = false;

		for (int i = 0; i < strlen(input); i++) {
			if (input[i] == '"') {
				insideQuotes = !insideQuotes;
			}
			if (input[i] == '|' && !insideQuotes) {
				hasPipe = true;
				break;
			}
		}

		if (hasPipe) {
			std::vector<Parser::ParsedCommand> parsedCommands = commandParser.parsePipeline(input);
			if (errorHandling->catchPipeErrors(input, parsedCommands)) {
				continue;
			}
			std::vector<Command*> commands;
			bool validationSuccess = true;

			for (auto parsedCommand : parsedCommands) {
				if (!errorHandling->validateCommand(parsedCommand)) {
					validationSuccess = false;
					break;
				}
				Command* command = factory.createCommand(parsedCommand.commandName, parsedCommand.commandOpt, parsedCommand.commandArg, parsedCommand.streams);
				commands.push_back(command);
			}

			if (validationSuccess) {
				Pipeline pipeline(commands);
				pipeline.execute();
			}
		}
		else {
			Parser::ParsedCommand parsedCommand = commandParser.parseCommand(input);
			if (errorHandling->catchErrors(input, parsedCommand) || !errorHandling->validateCommand(parsedCommand)) {
				continue;
			}

			Command* command = factory.createCommand(parsedCommand.commandName, parsedCommand.commandOpt, parsedCommand.commandArg, parsedCommand.streams);

			command->execute();
			delete command;
		}

		if (std::cin.eof()) {
			std::cin.clear();
			continue;
		}
	}

	delete errorHandling;

	return 0;
}