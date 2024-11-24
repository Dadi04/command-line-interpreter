#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <iostream>
#include <fstream>
#include "Command.h"
#include "CommandFactory.h"

class Parser {
public:
	Command* parseCommand(const std::string& input) {
		CommandFactory commandFactory;

		std::string commandName; std::string commandOpt; std::string commandArg;

		int i = 0; 
		bool inQuotes = false;

		// Preskace beline na pocetku
		while (i < input.length() && std::isspace(input[i])) {
			i++;
		}
		// Pronalazi ime komande
		while (i < input.length() && !std::isspace(input[i])) {
			commandName += input[i];
			i++;
		}
		// preskace beline izmedju komande i opcije
		while (i < input.length() && std::isspace(input[i])) {
			i++;
		}
		// pronalazi opciju ako postoji
		if (i < input.length() && input[i] == '-') {
			while (i < input.length() && !std::isspace(input[i])) {
				commandOpt += input[i];
				i++;
			}
		}
		// preskace beline izmedju opcije i argumenta
		while (i < input.length() && std::isspace(input[i])) {
			i++;
		}
		// // pronalazi argument ako postoji
		if (i < input.length()) {
			if (input[i] == '"') {
				inQuotes = true;
				i++;
			}
			while (i < input.length() && (inQuotes || !std::isspace(input[i]))) {
				if (input[i] == '"') {
					inQuotes = false;
					i++;
					break;
				}
				commandArg += input[i];
				i++;
			}
		}

		return commandFactory.createCommand(commandName, commandOpt, commandArg);
	}
};


#endif