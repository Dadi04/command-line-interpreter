#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command {
public:
	virtual ~Command() {}

	Command(std::string commandName, std::string opt, std::string arg) : name(commandName), option(opt), argument(arg) {}

	virtual void execute() = 0;
protected:
	// metoda koja odredjuje da li je argument string ili file
	std::string getArgumentType();
	std::string ifArgumentEmpty();

	std::string name;
	std::string option;
	std::string argument;
};

class Echo : public Command {
public:
	Echo(std::string arg) : Command("echo", "", arg) {};

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
	Touch(std::string arg) : Command("touch", "", arg) {}

	void execute();
};

class Wc : public Command {
public:
	Wc(std::string opt, std::string arg) : Command("wc", opt, arg) {}

	void execute();
};

#endif