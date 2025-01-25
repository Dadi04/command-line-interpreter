#include "ErrorHandling.h"
#include "Command.h"
#include <iostream>

bool ErrorHandling::catchLexicalError(std::string commandLine, Parser::ParsedCommand command) {
    int lineLength = commandLine.length();
    char* mistakes = new char[lineLength + 1];
    for (int i = 0; i < lineLength; i++) {
        mistakes[i] = ' ';
    }

    int counter = 0, nameSize = command.commandName.length(), optSize = command.commandOpt.length(), argSize = command.commandArg.length();
    bool hasErrors = false;

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

    if (!command.commandOpt.empty()) {
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
    }
    
    while (counter < lineLength && (std::isspace(commandLine[counter]) || commandLine[counter] == '\t')) {
        counter++;
    }

    if (!command.commandArg.empty() && commandLine[counter] != '<' && commandLine[counter] != '>') {
        if (counter < lineLength && command.commandArg.front() == '"' && command.commandArg.back() == '"') {
            for (int i = 0; i < argSize; i++) {
                counter++;
            }
        }
        else if (counter < lineLength && command.commandArg.find_first_of(" \t") == std::string::npos) {
            for (int i = 0; i < argSize; i++) {
                if (counter < lineLength && std::isspace(commandLine[counter])) {
                    mistakes[counter] = '^';
                    hasErrors = true;
                }
                counter++;
            }
        }
        else {
            for (int i = 0; i < argSize; i++) {
                if (counter < lineLength && commandLine[counter] != '"' && commandLine[counter] != '.' && !std::isalnum(commandLine[counter]) && !std::isspace(commandLine[counter]) && commandLine[counter] != '\t') {
                    mistakes[counter] = '^';
                    hasErrors = true;
                }
                counter++;
            }
        }
    }

    while (counter < lineLength && (std::isspace(commandLine[counter]) || commandLine[counter] == '\t')) {
        counter++;
    }

    if (!command.streams.empty()) {
        std::string streamFile1, streamFile2;
        if (command.streams.size() == 1) {
            streamFile1 = command.streams[0].file;
        }
        else if (command.streams.size() == 2) {
            streamFile2 = command.streams[1].file;
        }
        while (counter < lineLength) {
            if (commandLine[counter] == '>' || commandLine[counter] == '<') {
                char redirectionType = commandLine[counter];
                counter++;

                if (redirectionType == '>' && counter < lineLength && commandLine[counter] == '>') {
                    counter++;
                } 
             
                while (counter < lineLength && (std::isspace(commandLine[counter]) || commandLine[counter] == '\t')) {
                    counter++;
                }

                std::string fileName;
                if (redirectionType == '>') {
                    fileName = (!streamFile1.empty() ? streamFile1 : streamFile2);
                }
                else if (redirectionType == '<') {
                    fileName = (!streamFile2.empty() ? streamFile2 : streamFile1);
                }

                if (fileName.empty()) {
                    continue;
                }

                for (int i = 0; i < fileName.length(); i++) {
                    if (counter + i >= lineLength) break;
                    if (std::isspace(fileName[i])) {
                        mistakes[counter + i] = '^';
                        hasErrors = true;
                    }
                }
                counter += fileName.length();
            }
            else {
                break;
            }
        }
    }

    for (int i = counter; i < lineLength; i++) {
        mistakes[i] = '^';
        hasErrors = true;
    }

    if (hasErrors) {
        std::cerr << "Error - unexpected characters:" << std::endl << commandLine << std::endl;
        for (int i = 0; i < lineLength; i++) {
            std::cerr << mistakes[i];
        }
        std::cerr << std::endl;
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
