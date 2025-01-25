#ifndef COMMAND_H
#define COMMAND_H

#include "Redirection.h"
#include <string>
#include <vector>
#include <sstream>

class Command {
public:
	virtual ~Command() {}

	Command(std::string commandName, std::string opt, std::string arg, std::vector<Redirection> streams) : name(commandName), option(opt), argument(arg), streams(streams), buffer(nullptr) {}

	virtual void execute() = 0;
	virtual void print(std::string output) = 0;
	//virtual bool catchErrors() = 0;

	void setBuffer(std::stringstream* buffer);
	std::string getBuffer();

	std::vector<Redirection> getStreams();
protected:
	bool RedirectInput(std::string& input);
	bool RedirectOutput(std::string input);
	std::string getArgumentType();
	std::string ifArgumentEmpty();
	std::string ifBufferNotEmpty();

	std::string name;
	std::string option;
	std::string argument;
	std::vector<Redirection> streams;

	std::stringstream* buffer;
};

class Echo : public Command {
public:
	Echo(std::string arg, std::vector<Redirection> streams) : Command("echo", "", arg, streams) {};

	void execute();
	void print(std::string output);
};

class Prompt : public Command {
public:
	Prompt(std::string arg, std::vector<Redirection> streams) : Command("prompt", "", arg, streams) {};
	static std::string getPromptSign();

	void execute();
	void print(std::string output) {};
private:
	static std::string promptSign;
};

class Time : public Command {
public:
	Time(std::vector<Redirection> streams) : Command("time", "", "", streams) {}

	void execute();
	void print(std::string output);
};

class Date : public Command {
public:
	Date(std::vector<Redirection> streams) : Command("date", "", "", streams) {}

	void execute();
	void print(std::string output);
};

class Touch : public Command {
public:
	Touch(std::string arg, std::vector<Redirection> streams) : Command("touch", "", arg, streams) {}

	void execute();
	void print(std::string output) {};
};

class Truncate : public Command {
public:
	Truncate(std::string arg, std::vector<Redirection> streams) : Command("truncate", "", arg, streams) {}

	void execute();
	void print(std::string output) {};
};

class Rm : public Command {
public:
	Rm(std::string arg, std::vector<Redirection> streams) : Command("rm", "", arg, streams) {}

	void execute();
	void print(std::string output) {};
};

class Wc : public Command {
public:
	Wc(std::string opt, std::string arg, std::vector<Redirection> streams) : Command("wc", opt, arg, streams) {}

	void execute();
	void print(std::string output);
};

class Tr : public Command {
public:
	Tr(std::string arg, std::vector<Redirection> streams) : Command("tr", "", arg, streams) {
		parseArguments(arg);
	}

	void execute();
	void print(std::string output);
	std::string getWhat();
	std::string getWith();
private:
	std::string what;
	std::string with;

	void checkBuffer();
	void parseArguments(std::string arg);
};

class Head : public Command {
public:
	Head(std::string opt, std::string arg, std::vector<Redirection> streams) : Command("head", opt, arg, streams) {}

	void execute();
	void print(std::string output);
};


class Batch : public Command {
public:
	Batch(std::string arg, std::vector<Redirection> streams) : Command("batch", "", arg, streams) {}

	void execute();
	void print(std::string output) {};
};

#endif