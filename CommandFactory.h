#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include "Redirection.h"
#include <string>
#include <vector>

class Command;

class CommandFactory {
public:
	Command* createCommand(
		std::string name, 
		std::string opt, 
		std::string arg, 
		std::vector<Redirection> streams
	);
};

#endif