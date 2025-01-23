#include "Command.h"
#include <iostream>
#include <ctime>

void Time::execute() {
	time_t timestamp;
	time(&timestamp);
	struct tm currentTime;

	if (localtime_s(&currentTime, &timestamp) == 0) {
		char timeString[9];
		strftime(timeString, sizeof(timeString), "%H:%M:%S", &currentTime);

		print(timeString);
	}
}

void Time::print(std::string output) {
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
