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
	bool catchSyntaxError();
	bool catchSemanticError();
	bool catchPipeLexicalError(std::string commandLine, std::vector<Parser::ParsedCommand>);
	bool catchPipeSyntaxError();
	bool catchPipeSemanticError();
private:

};

#endif