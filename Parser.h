#ifndef PARSER_H
#define PARSER_H

#include "Redirection.h"
#include <string>
#include <vector>

class Parser {
public:
	struct ParsedCommand {
		std::string commandName;
		std::string commandOpt;
		std::string commandArg;
		std::vector<Redirection> streams;
	};

	ParsedCommand parseCommand(std::string input);
	std::vector<ParsedCommand> parsePipeline(std::string input);
private:
	void skipWhiteSpace(std::string input, int& i);
	std::string readToken(std::string input, int& i);
	std::string readArgument(std::string input, int& i);
	Redirection readStreamFile(std::string input, int& i);
	std::string readStreamFileName(std::string input, int& i);
};

#endif