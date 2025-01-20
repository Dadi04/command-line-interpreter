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

class Prompt : public Command {
public:
	Prompt(std::string arg) : Command("prompt", "", arg) {};
	static std::string getPromptSign();

	void execute();
private:
	static std::string promptSign;
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

class Truncate : public Command {
public:
	Truncate(std::string arg) : Command("truncate", "", arg) {}

	void execute();
};

class Rm : public Command {
public:
	Rm(std::string arg) : Command("rm", "", arg) {}

	void execute();
};

class Wc : public Command {
public:
	Wc(std::string opt, std::string arg) : Command("wc", opt, arg) {}

	void execute();
};

class Tr : public Command {
public:
	Tr(std::string arg) : Command("tr", "", arg) {
		parseArguments(arg);
	}

	void execute();
private:
	std::string what;
	std::string with;

	void parseArguments(std::string arg);
};

class Head : public Command {
public:
	Head(std::string opt, std::string arg) : Command("head", opt, arg) {}

	void execute();
};


class Batch : public Command {
public:
	Batch(std::string arg) : Command("batch", "", arg) {}

	void execute();
};

#endif