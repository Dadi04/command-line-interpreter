#include "Command.h"
#include <ctime>

void Date::execute() {
	time_t timestamp;
	time(&timestamp);
	struct tm currentDate;

	if (localtime_s(&currentDate, &timestamp) == 0) {
		char dateString[11];
		strftime(dateString, sizeof(dateString), "%d.%m.%Y", &currentDate);

		RedirectOutput(dateString);
	}
}

void Date::print() {

}
