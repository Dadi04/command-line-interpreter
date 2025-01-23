#ifndef PIPELINE_H
#define PIPELINE_H

#include "Command.h"
#include <string>
#include <vector>

class Pipeline {
public:
	Pipeline(std::vector<Command*> commands) : commands(commands) {}
	~Pipeline();

	void execute();
private:
	std::vector<Command*> commands;
};

#endif