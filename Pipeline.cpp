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

        // upitno ali resava problem; echo "goran" | time ne bi trebalo da radi, u sustini sve komande koje ne zahtevaju argument (time, date)
        if ((command->getName() == "time" || command->getName() == "date") && !command->getBuffer().empty()) {
            std::cerr << "The command does not accept piped input." << std::endl;
            return;
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
            commands.back()->setBuffer(nullptr);
            commands.back()->print(output);
        }
    }
}