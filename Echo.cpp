#include "Command.h"
#include <iostream>
#include <string>

void Echo::execute() {
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

	if (input.back() == '\n') {
		input.pop_back();
	}

	print(input);
}

void Echo::print(std::string output) {
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

	for (Redirection& stream : streams) {
		if (stream.type != Redirection::Output && stream.type != Redirection::Append) {
			std::string output = buffer->str();
			if (!output.empty()) {
				if (output.front() == '"' && output.back() == '"') {
					output = output.substr(1, output.size() - 2);
				}
				buffer->str("");
				buffer->clear();
				std::cout << output << std::endl;
				return;
			}
		}
	}

	/*if (streams.empty() && buffer && !buffer->str().empty()) {
		std::string output = buffer->str();
		if (!output.empty()) {
			if (output.front() == '"' && output.back() == '"') {
				output = output.substr(1, output.size() - 2);
			}
			buffer->str("");
			buffer->clear();
			std::cout << output << std::endl;
		}
	}*/
}