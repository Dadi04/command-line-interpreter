#include "Command.h"
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

	RedirectOutput(input);
}

void Echo::print() {

}
