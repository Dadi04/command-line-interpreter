#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>

const int MAX_INPUT_LENGHT = 512;

class Interpreter {
public:
	void run();

private:
	std::string prompt = "$ ";
};


#endif