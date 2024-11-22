#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command {
public:
	virtual ~Command();

	Command(const std::string& commandName, const std::string& opt = "", const std::string& arg = "") : name(commandName), option(opt), argument(arg) {}

	virtual void execute() = 0;
	virtual bool isCorrect(std::string name, std::string option, std::string argument) = 0;

	std::string getName() { return name; }
	std::string getOption() { return option; }
	std::string getArgument() { return argument; }
protected:
	std::string name;
	std::string option;
	std::string argument;
};

// svaka ova komanda mora da se pojedinacno proveri 
// moram da razmislim kako ce to da radi
class Echo : public Command {
	Echo();
	virtual bool isCorrect();
};

class Time : public Command {
	Time();
	virtual bool isCorrect();
};

class Date : public Command {
	Date();
	virtual bool isCorrect();
};

class Touch	: public Command {
	Touch();
	virtual bool isCorrect();
};

class Wc : public Command {
	Wc();
	virtual bool isCorrect();
};

#endif