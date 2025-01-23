#include "Command.h"
#include <iostream>
#include <ctime>

void Date::execute() {
	time_t timestamp;
	time(&timestamp);
	struct tm currentDate;

	if (localtime_s(&currentDate, &timestamp) == 0) {
		char dateString[11];
		strftime(dateString, sizeof(dateString), "%d.%m.%Y", &currentDate);

		print(dateString);
	}
}

void Date::print(std::string output) {
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
