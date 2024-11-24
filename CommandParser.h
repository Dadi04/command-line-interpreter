#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include <iostream>
#include <fstream>

class CommandParser {
public:
	std::string* parse(const std::string& input) {
		int count = 0;
		
		for (int i = 0; input[i] != '\0'; i++) {
			if (std::isspace(input[i])) {
				count++;
			}
		}
		count++;

		if (count == 0) return nullptr;

		std::string* parsedInput = new std::string[count];

		int inputIndex = 0;
		int wordIndex = 0;
		while (input[inputIndex] != '\0' && wordIndex < count) {
			while (std::isspace(input[inputIndex])) inputIndex++;

			if (input[inputIndex] == '\0') break;

			int start = inputIndex;
			while (input[inputIndex] != '\0' && !std::isspace(input[inputIndex])) {
				inputIndex++;
			}
			int end = inputIndex;

			parsedInput[wordIndex++] = input.substr(start, end - start);
		}

		return parsedInput;
	}
};


#endif