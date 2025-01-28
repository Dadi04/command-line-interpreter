#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

#include "Parser.h"
#include "Command.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

class ErrorHandling {
public:
	~ErrorHandling() {};
	ErrorHandling() {
		validators = {
			{"echo", [this](Parser::ParsedCommand parsedCommand) { return validateEcho(parsedCommand); }},
			{"prompt", [this](Parser::ParsedCommand parsedCommand) { return validatePrompt(parsedCommand); }},
			{"time", [this](Parser::ParsedCommand parsedCommand) { return validateTime(parsedCommand); }},
			{"date", [this](Parser::ParsedCommand parsedCommand) { return validateDate(parsedCommand); }},
			{"touch", [this](Parser::ParsedCommand parsedCommand) { return validateTouch(parsedCommand); }},
			{"truncate", [this](Parser::ParsedCommand parsedCommand) { return validateTruncate(parsedCommand); }},
			{"rm", [this](Parser::ParsedCommand parsedCommand) { return validateRm(parsedCommand); }},
			{"wc", [this](Parser::ParsedCommand parsedCommand) { return validateWc(parsedCommand); }},
			{"tr", [this](Parser::ParsedCommand parsedCommand) { return validateTr(parsedCommand); }},
			{"head", [this](Parser::ParsedCommand parsedCommand) { return validateHead(parsedCommand); }},
			{"batch", [this](Parser::ParsedCommand parsedCommand) { return validateBatch(parsedCommand); }}
		};
	};

	bool validateCommand(Parser::ParsedCommand parsedCommand);

	bool catchLexicalError(std::string commandLine, Parser::ParsedCommand command);
	bool catchPipeLexicalError(std::string commandLine, std::vector<Parser::ParsedCommand> commands);

private:
	std::unordered_map<std::string, std::function<bool(Parser::ParsedCommand)>> validators;

	void skipWhiteSpace(std::string commandLine, int& index);
	bool validateCommandName(std::string commandLine, std::string name, char* mistakes, int& index);
	bool validateCommandOption(std::string commandLine, std::string option, char* mistakes, int& index);
	bool validateCommandArgument(std::string commandLine, std::string argument, char* mistakes, int& index);
	bool validateStreams(std::string commandLine, std::vector<Redirection> streams, char* mistakes, int& index);

	bool validateCommandArgumentTr(std::string commandLine, std::string argument, char* mistakes, int& index);
	bool validateCommandArgumentBatch(std::string commandLine, std::string argument, char* mistakes, int& index);

	bool validateEcho(Parser::ParsedCommand parsedCommand);
	bool validatePrompt(Parser::ParsedCommand parsedCommand);
	bool validateTime(Parser::ParsedCommand parsedCommand);
	bool validateDate(Parser::ParsedCommand parsedCommand);
	bool validateTouch(Parser::ParsedCommand parsedCommand);
	bool validateTruncate(Parser::ParsedCommand parsedCommand);
	bool validateRm(Parser::ParsedCommand parsedCommand);
	bool validateWc(Parser::ParsedCommand parsedCommand);
	bool validateTr(Parser::ParsedCommand parsedCommand);
	bool validateHead(Parser::ParsedCommand parsedCommand);
	bool validateBatch(Parser::ParsedCommand parsedCommand);
};

#endif