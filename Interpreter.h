#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Parser.h"
#include "Command.h"
#include "CommandFactory.h"
#include <string>

class Interpreter {
public:
	void run();
};

#endif