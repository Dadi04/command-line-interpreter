#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H
#include "Parser.h"
#include <string>
#include <vector>

class ErrorHandling {
public:
	~ErrorHandling() {};
	ErrorHandling() {};

	bool catchLexicalError(std::string commandLine, Parser::ParsedCommand);
	bool catchPipeLexicalError(std::string commandLine, std::vector<Parser::ParsedCommand>);
private:
	void skipWhiteSpace(std::string commandLine, int& index);
	bool validateCommandName(std::string commandLine, std::string name, char* mistakes, int& index);
	bool validateCommandOption(std::string commandLine, std::string option, char* mistakes, int& index);
	bool validateCommandArgument(std::string commandLine, std::string argument, char* mistakes, int& index);
	bool validateStreams(std::string commandLine, std::vector<Redirection> streams, char* mistakes, int& index);
};

#endif