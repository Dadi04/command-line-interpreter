#ifndef PARSER_H
#define PARSER_H

#include "Command.h"
#include <string>

class Parser {
public:
	struct ParsedCommand {
		std::string commandName;
		std::string commandOpt;
		std::string commandArg;
	};

	ParsedCommand parseCommand(std::string input);
};


#endif