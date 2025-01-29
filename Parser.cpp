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

std::vector<Parser::ParsedCommand> Parser::parsePipeline(std::string input) {
	std::vector<ParsedCommand> parsedCommands;
	int pos = 0;
	bool insideQuotes = false;

	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '"') {
			insideQuotes = !insideQuotes;
		}
		else if (input[i] == '|' && !insideQuotes) {
			std::string segment = input.substr(pos, i - pos);
			parsedCommands.push_back(parseCommand(segment));
			pos = i + 1;
		}
	}

	if (pos < input.length()) {
		parsedCommands.push_back(parseCommand(input.substr(pos)));
	}

	return parsedCommands;
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
	bool insideQuotes = false;

	while (i < input.length()) {
		char c = input[i];

		if ((c == '<' || c == '>') && !insideQuotes) {
			break;
		}

		if (c == '"') {
			insideQuotes = !insideQuotes;
			token += c;
		}
		else {
			token += c;
		}

		i++;
	}
	if (!token.empty() && std::isspace(token.back())) {
		token.pop_back();
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
	redirection.file = readStreamFileName(input, i);

	return redirection;
}

std::string Parser::readStreamFileName(std::string input, int& i) {
	std::string token;
	while (i < input.length() && input[i] != '<' && input[i] != '>') {
		token += input[i];
		i++;
	}
	if (!token.empty() && token.back() == ' ') {
		token.pop_back();
	}
	return token;
}

