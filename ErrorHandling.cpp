#include "ErrorHandling.h"
#include <iostream>

bool ErrorHandling::catchLexicalError(std::string commandLine, Parser::ParsedCommand command) {
    int lineLength = commandLine.length();
    char* mistakes = new char[lineLength + 1];
    int counter = 0;
    int nameSize = command.commandName.length(), optSize = command.commandOpt.length(), argSize = command.commandArg.length();
    bool hasErrors = false;

    for (int i = 0; i < lineLength; i++) {
        mistakes[i] = ' ';
    }

    while (counter < lineLength && (std::isspace(commandLine[counter]) || commandLine[counter] == '\t')) {
        counter++;
    }

    for (int i = 0; i < nameSize; i++) {
        if (counter < lineLength && !std::isalpha(command.commandName[i])) {
            mistakes[counter] = '^';
            hasErrors = true;
        }
        counter++;
    }

    while (counter < lineLength && (std::isspace(commandLine[counter]) || commandLine[counter] == '\t')) {
        counter++;
    }

    if (counter < lineLength && commandLine[counter] == '-') {
        counter++;
        for (int i = 1; i < optSize; i++) {
            if (counter < lineLength && !std::isalpha(command.commandOpt[i]) && !std::isdigit(command.commandOpt[i])) {
                mistakes[counter] = '^';
                hasErrors = true;
            }
            counter++;
        }
    }
    else if (counter < lineLength) {
        mistakes[counter] = '^';
        hasErrors = true;
        counter++;
    }

    while (counter < lineLength && (std::isspace(commandLine[counter]) || commandLine[counter] == '\t')) {
        counter++;
    }

    if (counter < lineLength && commandLine[counter] != '"') {
        mistakes[counter] = '^';
        hasErrors = true;
        counter++;
    }

    if (counter < lineLength && commandLine[counter] == '"') {
        counter++;
        for (int i = 1; i < argSize; i++) {
            if (counter < lineLength && commandLine[counter] == '"') {
                counter++;
                break;
            }
            counter++;
        }
    }

    while (counter < lineLength && (std::isspace(commandLine[counter]) || commandLine[counter] == '\t')) {
        counter++;
    }

    for (int i = counter; i < lineLength; i++) {
        mistakes[i] = '^';
        hasErrors = true;
    }

    if (hasErrors) {
        std::cout << "Error - unexpected characters:\n" << commandLine << "\n";
        for (int i = 0; i < lineLength; i++) {
            std::cout << mistakes[i];
        }
        std::cout << std::endl;
    }

    delete[] mistakes;
    return hasErrors;
}

bool ErrorHandling::catchSyntaxError() {
	return true;
}

bool ErrorHandling::catchSemanticError() {
	return true;
}

bool ErrorHandling::catchPipeLexicalError(std::string commandLine, std::vector<Parser::ParsedCommand>) {
	return true;
}

bool ErrorHandling::catchPipeSyntaxError() {
	return true;
}

bool ErrorHandling::catchPipeSemanticError() {
	return true;
}
