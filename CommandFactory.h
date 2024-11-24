#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include <string>
#include "Command.h"

class CommandFactory {
public:
	Command* createCommand(const std::string& name, const std::string& opt, const std::string& arg);
};


#endif