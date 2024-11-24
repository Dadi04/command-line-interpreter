#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

// treba da napravim metodu koja proverava opcije, trebad a napravim metodu koja proverava argumente, i metodu koja proverava da li je komanda korektna

class Command {
public:
	~Command() {}

	Command(const std::string& commandName, const std::string& opt = "", const std::string& arg = "") : name(commandName), option(opt), argument(arg) {}

	virtual void execute() = 0;

	std::string getName() { return name; }
	std::string getOption() { return option; }
	std::string getArgument() { return argument; }
protected:
	std::string name;
	std::string option;
	std::string argument;
};

class Echo : public Command {
public:
	Echo(const std::string& arg) : Command("echo", "", arg) {}

	void execute() {
		std::cout << argument << std::endl;
	}
};

class Time : public Command {
public:
	Time() : Command("time", "", "") {}

	void execute() {
		time_t timestamp;
		time(&timestamp);
		struct tm currentTime;

		if (localtime_s(&currentTime, &timestamp) == 0) {
			char timeString[9];
			strftime(timeString, sizeof(timeString), "%H:%M:%S", &currentTime);

			std::cout << timeString << std::endl;
		}
		else {
			std::cerr << "Greska pri dobijanju lokalnog vremena." << std::endl;
		}
	}
};

class Date : public Command {
public:
	Date() : Command("date", "", "") {}

	void execute() {
		time_t timestamp;
		time(&timestamp);
		struct tm currentDate;

		if (localtime_s(&currentDate, &timestamp) == 0) {
			char dateString[11];
			strftime(dateString, sizeof(dateString), "%Y-%m-%d", &currentDate);

			std::cout << dateString << std::endl;
		}
		else {
			std::cerr << "Greska pri dobijanju lokalnog datuma." << std::endl;
		}
		
	}
};

class Touch	: public Command {
public:
	Touch(const std::string& arg) : Command("touch", "", arg) {}

	void execute() {
		std::ifstream file(argument);
		if (file) {
			std::cerr << "Greska: Datoteka \"" << argument << "\" vec postoji." << std::endl;
		}
		else {
			std::ofstream newFile(argument);
			if (newFile) {
				newFile.close();
				std::cout << "Datoteka \"" << argument << "\" je uspesno kreairana." << std::endl;
			}
			else {
				std::cerr << "Greska pri kreairanju datoteke \"" << argument << "\"." << std::endl;
			}
		}
	}
};

class Wc : public Command {
public:
	Wc(const std::string& opt, const std::string& arg) : Command("wc", opt, arg) {}

	void execute() {
		if (option == "-w") {
			int count = 0;
			for (int i = 0; i < argument.length(); i++) {
				if (std::isspace(argument[i])) {
					count++;
				}
			}
			count++;

			std::cout << count << std::endl;
		}
		else if (option == "-c") {
			int count = 0;
			for (int i = 0; i < argument.length(); i++) {
				count++;
			}
			std::cout << count << std::endl;
		}
		else {
			std::cerr << "Ne postoji takva opcija." << std::endl;
		}
	}
};

#endif