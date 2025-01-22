#include "Interpreter.h"
#include "Parser.h"
#include "Command.h"
#include "CommandFactory.h"
#include "Pipeline.h"
#include <string>
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
			std::cerr << "Input exceeds the maximum length of " << MAX_INPUT_LENGTH << " characters." << std::endl;
			continue;
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
			std::vector<Command*> commands;

			for (auto parsedCommand : parsedCommands) {
				Command* command = commandFactory.createCommand(parsedCommand.commandName, parsedCommand.commandOpt, parsedCommand.commandArg, parsedCommand.streams);
				if (!command) {
					std::cerr << "Unknown command: \"" << parsedCommand.commandName << "\"" << std::endl;
					break;
				}
				commands.push_back(command);
			}

			Pipeline pipeline(commands);
			pipeline.execute();
		}
		else {
			Parser::ParsedCommand parsedCommand = commandParser.parseCommand(input);
			Command* command = commandFactory.createCommand(parsedCommand.commandName, parsedCommand.commandOpt, parsedCommand.commandArg, parsedCommand.streams);

			if (!command) {
				std::cerr << "Unknown command: \"" << parsedCommand.commandName << "\"" << std::endl;
				continue;
			}

			command->execute();
			delete command;
		}

		if (std::cin.eof()) {
			std::cin.clear();
			continue;
		}
	}
}