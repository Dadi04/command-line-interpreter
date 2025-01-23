#include "Pipeline.h"
#include "Command.h"
#include <string>
#include <iostream>

Pipeline::~Pipeline() {
	for (auto command : commands) {
		delete command;
	}
}

void Pipeline::execute() {
    std::stringstream buffer;

    for (int i = 0; i < commands.size(); i++) {
        Command* command = commands[i];

        if (i < commands.size() - 1) {
            command->setBuffer(&buffer);
        }

        command->execute();

        if (i < commands.size() - 1) {
            commands[i + 1]->setBuffer(&buffer);
        }
    }

    std::vector<Redirection> streams = commands.back()->getStreams();
    if (!commands.empty() && streams.empty()) {
        std::string output = commands.back()->getBuffer();
        if (!output.empty() && output.front() == '"' && output.back() == '"') {
            output = output.substr(1, output.size() - 2);
            std::cout << output << std::endl;
        }
    }
}