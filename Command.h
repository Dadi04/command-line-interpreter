#ifndef COMMAND_H
#define COMMAND_H

#include "Redirection.h"
#include <string>
#include <vector>

class Command {
public:
	virtual ~Command() {}

	Command(std::string commandName, std::string opt, std::string arg, std::vector<Redirection> streams) : name(commandName), option(opt), argument(arg), streams(streams) {}

	virtual void execute() = 0;
protected:
	// metoda koja odredjuje da li je argument string ili file
	std::string getArgumentType();
	std::string ifArgumentEmpty();
	void RedirectInput(std::string& input);
	void RedirectOutput(std::string input);

	std::string name;
	std::string option;
	std::string argument;
	std::vector<Redirection> streams;
};

class Echo : public Command {
public:
	Echo(std::string arg, std::vector<Redirection> streams) : Command("echo", "", arg, streams) {};

	void execute();
};

class Prompt : public Command {
public:
	Prompt(std::string arg, std::vector<Redirection> streams) : Command("prompt", "", arg, streams) {};
	static std::string getPromptSign();

	void execute();
private:
	static std::string promptSign;
};

class Time : public Command {
public:
	Time(std::vector<Redirection> streams) : Command("time", "", "", streams) {}

	void execute();
};

class Date : public Command {
public:
	Date(std::vector<Redirection> streams) : Command("date", "", "", streams) {}

	void execute();
};

class Touch	: public Command {
public:
	Touch(std::string arg, std::vector<Redirection> streams) : Command("touch", "", arg, streams) {}

	void execute();
};

class Truncate : public Command {
public:
	Truncate(std::string arg, std::vector<Redirection> streams) : Command("truncate", "", arg, streams) {}

	void execute();
};

class Rm : public Command {
public:
	Rm(std::string arg, std::vector<Redirection> streams) : Command("rm", "", arg, streams) {}

	void execute();
};

class Wc : public Command {
public:
	Wc(std::string opt, std::string arg, std::vector<Redirection> streams) : Command("wc", opt, arg, streams) {}

	void execute();
};

class Tr : public Command {
public:
	Tr(std::string arg, std::vector<Redirection> streams) : Command("tr", "", arg, streams) {
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
	Head(std::string opt, std::string arg, std::vector<Redirection> streams) : Command("head", opt, arg, streams) {}

	void execute();
};


class Batch : public Command {
public:
	Batch(std::string arg, std::vector<Redirection> streams) : Command("batch", "", arg, streams) {}

	void execute();
};

#endif