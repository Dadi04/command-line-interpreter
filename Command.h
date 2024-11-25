#ifndef COMMAND_H
#define COMMAND_H

#include "ErrorHandler.h"
#include <string>

class Command {
public:
	~Command() {}

	Command(std::string commandName, std::string opt, std::string arg) : name(commandName), option(opt), argument(arg) {}

	virtual void execute() = 0;

	std::string getArgumentInput();
protected:
	std::string name;
	std::string option;
	std::string argument;
};

class Echo : public Command {
public:
	Echo(const std::string& arg) : Command("echo", "", arg) {}

	void execute();
};

class Time : public Command {
public:
	Time() : Command("time", "", "") {}

	void execute();
};

class Date : public Command {
public:
	Date() : Command("date", "", "") {}

	void execute();
};

class Touch	: public Command {
public:
	Touch(const std::string& arg) : Command("touch", "", arg) {}

	void execute();
};

class Wc : public Command {
public:
	Wc(const std::string& opt, const std::string& arg) : Command("wc", opt, arg) {}

	void execute();
};

#endif