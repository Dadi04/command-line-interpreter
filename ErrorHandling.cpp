#include "ErrorHandling.h"
#include "Command.h"
#include <iostream>

bool ErrorHandling::catchLexicalError(std::string commandLine, Parser::ParsedCommand command) {
    int lineLength = commandLine.length();
    char* mistakes = new char[lineLength + 1];
    for (int i = 0; i < lineLength; i++) {
        mistakes[i] = ' ';
    }

    int index = 0, nameSize = command.commandName.length(), optSize = command.commandOpt.length(), argSize = command.commandArg.length();
    bool hasErrors = false;

    skipWhiteSpace(commandLine, index);

    hasErrors |= validateCommandName(commandLine, command.commandName, mistakes, index);

    skipWhiteSpace(commandLine, index);

    hasErrors |= validateCommandOption(commandLine, command.commandOpt, mistakes, index);

    skipWhiteSpace(commandLine, index);

    hasErrors |= validateCommandArgument(commandLine, command.commandArg, mistakes, index);

    skipWhiteSpace(commandLine, index);

    hasErrors |= validateStreams(commandLine, command.streams, mistakes, index);

    for (int i = index; i < lineLength; i++) {
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

bool ErrorHandling::catchPipeLexicalError(std::string commandLine, std::vector<Parser::ParsedCommand> commands) {
    for (int i = 0; i < commands.size(); i++) {
        std::cout << "Name: " << commands[i].commandName << ", Option: " << commands[i].commandOpt << ", Argument: " << commands[i].commandArg << std::endl;
    }
    return false;
}

void ErrorHandling::skipWhiteSpace(std::string commandLine, int& index) {
    while (index < commandLine.length() && (std::isspace(commandLine[index]) || commandLine[index] == '\t')) {
        index++;
    }
}

bool ErrorHandling::validateCommandName(std::string commandLine, std::string name, char* mistakes, int& index) {
    bool hasErrors = false;
    for (int i = 0; i < name.length(); i++) {
        if (index < commandLine.length() && !std::isalpha(name[i])) {
            mistakes[index] = '^';
            hasErrors = true;
        }
        index++;
    }
    return hasErrors;
}

bool ErrorHandling::validateCommandOption(std::string commandLine, std::string option, char* mistakes, int& index) {
    bool hasErrors = false;
    if (!option.empty()) {
        if (index < commandLine.length() && commandLine[index] == '-') {
            index++;
            for (int i = 1; i < option.length(); i++) {
                if (index < commandLine.length() && !std::isalpha(option[i]) && !std::isdigit(option[i])) {
                    mistakes[index] = '^';
                    hasErrors = true;
                }
                index++;
            }
        }
        else if (index < commandLine.length()) {
            mistakes[index] = '^';
            hasErrors = true;
            index++;
        }
    }
    return hasErrors;
}

bool ErrorHandling::validateCommandArgument(std::string commandLine, std::string argument, char* mistakes, int& index) {
    bool hasErrors = false;
    if (!argument.empty() && commandLine[index] != '<' && commandLine[index] != '>') {
        if (index < commandLine.length() && argument.front() == '"' && argument.back() == '"') {
            for (int i = 0; i < argument.length(); i++) {
                index++;
            }
        }
        else if (index < commandLine.length() && argument.find_first_of(" \t") == std::string::npos) {
            for (int i = 0; i < argument.length(); i++) {
                if (index < commandLine.length() && std::isspace(commandLine[index])) {
                    mistakes[index] = '^';
                    hasErrors = true;
                }
                index++;
            }
        }
        else {
            for (int i = 0; i < argument.length(); i++) {
                if (index < commandLine.length() && commandLine[index] != '"' && commandLine[index] != '.' && !std::isalnum(commandLine[index]) && !std::isspace(commandLine[index]) && commandLine[index] != '\t') {
                    mistakes[index] = '^';
                    hasErrors = true;
                }
                index++;
            }
        }
    }
    return hasErrors;
}

bool ErrorHandling::validateStreams(std::string commandLine, std::vector<Redirection> streams, char* mistakes, int& index) {
    bool hasErrors = false;
    if (!streams.empty()) {
        std::string streamFile1, streamFile2;
        if (streams.size() == 1) {
            streamFile1 = streams[0].file;
        }
        else if (streams.size() == 2) {
            streamFile2 = streams[1].file;
        }
        while (index < commandLine.length()) {
            if (commandLine[index] == '>' || commandLine[index] == '<') {
                char redirectionType = commandLine[index];
                index++;

                if (redirectionType == '>' && index < commandLine.length() && commandLine[index] == '>') {
                    index++;
                }

                skipWhiteSpace(commandLine, index);

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
                    if (index + i >= commandLine.length()) break;
                    if (std::isspace(fileName[i])) {
                        mistakes[index + i] = '^';
                        hasErrors = true;
                    }
                }
                index += fileName.length();
            }
            else {
                break;
            }
        }
    }
    return hasErrors;
}
