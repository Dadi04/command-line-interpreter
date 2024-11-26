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
private:
	void skipWhiteSpace(std::string input, int& i);
	std::string readToken(std::string input, int& i);
	std::string readArgument(std::string input, int& i);
};


#endif