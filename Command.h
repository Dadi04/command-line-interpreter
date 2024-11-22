#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

class Command {
public:
	virtual ~Command();

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
private:
	Echo(const std::string& arg) : Command("echo", "", arg) {}
	void execute() {
		std::cout << argument << std::endl;
	}
};

class Time : public Command {
public:
	Time() : Command("time", "", "") {}
	void execute() {
		// implementirati vreme
	}
};

class Date : public Command {
public:
	Date() : Command("date", "", "") {}
	void execute() {
		// implementirati datum
	}
};

class Touch	: public Command {
public:
	Touch(const std::string& arg) : Command("touch", "", arg) {}
	void execute() {
		// implementirati kako napraviti file
	}
};

class Wc : public Command {
public:
	Wc(const std::string& opt, const std::string& arg) : Command("wc", opt, arg) {}
	void execute() {
		// implementirati word count
	}
};

#endif