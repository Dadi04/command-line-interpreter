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
}

// parseArgument za komandu tr napravi sranje kada nema argumenta tj kada je argument prazan, kada se koristi u pipeline