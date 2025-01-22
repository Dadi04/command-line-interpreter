#include "Command.h"
#include <string>

void Prompt::execute() {
	ifBufferNotEmpty();
	this->promptSign = argument + " ";
}

void Prompt::print() {

}

std::string Prompt::promptSign = "$ ";
std::string Prompt::getPromptSign() {
	return promptSign;
}
