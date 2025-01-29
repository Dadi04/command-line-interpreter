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

	bool catchErrors(std::string commandLine, Parser::ParsedCommand command);
	bool catchPipeErrors(std::string commandLine, std::vector<Parser::ParsedCommand> commands);

private:
	std::unordered_map<std::string, std::function<void(Parser::ParsedCommand)>> validators;

	void skipWhiteSpace(std::string commandLine, int& index);
	void validateCommandName(std::string commandLine, std::string name, char* mistakes, int& index);
	void validateCommandOption(std::string commandLine, std::string option, char* mistakes, int& index);
	void validateCommandArgument(std::string commandLine, std::string argument, char* mistakes, int& index);
	void validateStreams(std::string commandLine, std::vector<Redirection> streams, char* mistakes, int& index);

	void validateCommandArgumentTr(std::string commandLine, std::string argument, char* mistakes, int& index);
	void validateCommandArgumentBatch(std::string commandLine, std::string argument, char* mistakes, int& index);

	void validateEcho(Parser::ParsedCommand parsedCommand);
	void validatePrompt(Parser::ParsedCommand parsedCommand);
	void validateTime(Parser::ParsedCommand parsedCommand);
	void validateDate(Parser::ParsedCommand parsedCommand);
	void validateTouch(Parser::ParsedCommand parsedCommand);
	void validateTruncate(Parser::ParsedCommand parsedCommand);
	void validateRm(Parser::ParsedCommand parsedCommand);
	void validateWc(Parser::ParsedCommand parsedCommand);
	void validateTr(Parser::ParsedCommand parsedCommand);
	void validateHead(Parser::ParsedCommand parsedCommand);
	void validateBatch(Parser::ParsedCommand parsedCommand);
};

#endif