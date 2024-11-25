#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include "Command.h"
#include <string>

class CommandFactory {
public:
	Command* createCommand(std::string name, std::string opt, std::string arg);
};

#endif