#include "Command.h"
#include <string>

void Prompt::execute() {
	ifBufferNotEmpty();
	if (!argument.empty()) {
		if (argument.front() == '"' && argument.back() == '"') {
			argument = argument.substr(1, argument.size() - 2);
		}
	}
	this->promptSign = argument + " ";
}

std::string Prompt::promptSign = "$ ";
std::string Prompt::getPromptSign() {
	return promptSign;
}
