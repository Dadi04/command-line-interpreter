#include "Parser.h"
#include <iostream>

Parser::ParsedCommand Parser::parseCommand(std::string input) {
	ParsedCommand parsedCommand;
	int i = 0;

	skipWhiteSpace(input, i);

	parsedCommand.commandName = readToken(input, i);

	skipWhiteSpace(input, i);

	if (i < input.length() && input[i] == '-') {
		parsedCommand.commandOpt = readToken(input, i);
	}

	skipWhiteSpace(input, i);

	if (i < input.length() && input[i] != '<' && input[i] != '>') {
		parsedCommand.commandArg = readArgument(input, i);
	}

	skipWhiteSpace(input, i);

	while (i < input.length()) {
		Redirection redirection = readStreamFile(input, i);
		if (redirection.type != Redirection::StreamType::None) {
			parsedCommand.streams.push_back(redirection);
		}
		skipWhiteSpace(input, i);

		if (redirection.type == Redirection::StreamType::Input && parsedCommand.commandArg.empty()) {
			parsedCommand.commandArg = redirection.file;
		}
	}

	return parsedCommand;
}

void Parser::skipWhiteSpace(std::string input, int& i) {
	while (i < input.length() && (std::isspace(input[i]) || input[i] == '\t')) {
		i++;
	}
}

std::string Parser::readToken(std::string input, int& i) {
	std::string token;
	while (i < input.length() && !std::isspace(input[i]) && input[i] != '\t') {
		token += input[i];
		i++;
	}
	return token;
}

std::string Parser::readArgument(std::string input, int& i) {
	std::string token;

	if (i < input.length() && input[i] == '"') {
		i++;

		while (i < input.length()) {
			if (input[i] == '"') {
				size_t nextQuote = input.find('"', i + 1);
				if (nextQuote == std::string::npos) {
					break;
				}
			}
			token += input[i];
			i++;
		}

		if (i < input.length() && input[i] == '"') {
			i++;
		}

		return "\"" + token + "\"";
	}

	while (i < input.length()) {
		token += input[i];
		i++;
	}

	return token;
}

Redirection Parser::readStreamFile(std::string input, int& i) {
	Redirection redirection = { Redirection::StreamType::None, "" };

	skipWhiteSpace(input, i);
	if (i >= input.length()) return redirection;

	if (input[i] == '<') {
		redirection.type = Redirection::StreamType::Input;
		i++;
	}
	else if (input[i] == '>') {
		i++;
		if (i < input.length() && input[i] == '>') {
			redirection.type = Redirection::StreamType::Append;
			i++;
		}
		else {
			redirection.type = Redirection::StreamType::Output;
		}
	}
	else {
		return redirection;
	}

	skipWhiteSpace(input, i);
	redirection.file = readToken(input, i);

	return redirection;
}
