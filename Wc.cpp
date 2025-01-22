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

	RedirectInput(input);

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
		RedirectOutput(wordCountString);
	}
	else if (option == "-c") {
		std::string CharacterCountString = std::to_string(input.length());
		RedirectOutput(CharacterCountString);
	}
	else {
		std::cerr << "Error: Command wc must have either option -w or -c." << std::endl;
	}
}

void Wc::print() {

}
