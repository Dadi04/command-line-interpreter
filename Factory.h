#ifndef FACTORY_H
#define FACTORY_H

#include "Command.h"
#include "Redirection.h"
#include <string>
#include <vector>

class Factory {
public:
	Command* createCommand(std::string name, std::string opt, std::string arg, std::vector<Redirection> streams);
};

#endif