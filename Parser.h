#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "Command.h"

class Parser {
public:
	Command* parseCommand(const std::string& input);
};


#endif