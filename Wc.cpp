#include "Command.h"
#include <iostream>
#include <string>

void Wc::execute() {
	std::string input;

	ifBufferNotEmpty();
	if (argument.empty()) {
		input = ifArgumentEmpty();
	}
	else {
		input = getArgumentType();
		if (input.empty()) return;
	}

	if (option == "-w") {
		int wordCount = 0;
		bool inWord = false;
		for (int i = 0; i < input.length(); i++) {
			if (std::isspace(input[i])) {
				wordCount++;
				inWord = false;
			}
			else {
				inWord = true;
			}
		}
		if (inWord) {
			wordCount++;
		}

		std::string wordCountString = std::to_string(wordCount);
		print(wordCountString);
	}
	else if (option == "-c") {
		std::string CharacterCountString = std::to_string(input.length());
		print(CharacterCountString);
	}
	else {
		std::cerr << "Error: Command wc must have either option -w or -c." << std::endl;
	}
}

void Wc::print(std::string output) {
	if (!RedirectOutput(output)) {
		if (buffer) {
			buffer->str("");
			buffer->clear();
			*buffer << "\"" + output + "\"";
		}
		else {
			std::cout << output << std::endl;
		}
	}
}
