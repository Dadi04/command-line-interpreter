#include "ErrorHandling.h"
#include "Command.h"
#include <iostream>

bool ErrorHandling::validateCommand(Parser::ParsedCommand parsedCommand) {
    auto it = validators.find(parsedCommand.commandName);
    if (it == validators.end()) {
        std::cerr << "Unknown command: " << parsedCommand.commandName << std::endl;
        return false;
    }
    return it->second(parsedCommand);
}

bool ErrorHandling::catchLexicalError(std::string commandLine, Parser::ParsedCommand command) {
    int lineLength = commandLine.length();
    char* mistakes = new char[lineLength + 1];
    for (int i = 0; i < lineLength; i++) {
        mistakes[i] = ' ';
    }

    int index = 0;
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

// needs fix
bool ErrorHandling::catchPipeLexicalError(std::string commandLine, std::vector<Parser::ParsedCommand> commands) {
    int lineLength = commandLine.length();
    char* mistakes = new char[lineLength + 1];
    for (int i = 0; i < lineLength; i++) {
        mistakes[i] = ' ';
    }

    int index = 0;
    bool hasErrors = false;
    for (int i = 0; i < commands.size(); i++) {
        std::string partOfCommandLine;
        bool insideQuotes = false;
        for (int j = index; j < commandLine.length(); j++) {
            if (commandLine[j] == '"') {
                insideQuotes = !insideQuotes;
            }
            if (commandLine[j] == '|' && !insideQuotes) {
                index++;
                break;
            }
            partOfCommandLine += commandLine[j];
            index++;
        }

        // napraviti poseban lexical za pipe
        hasErrors |= catchLexicalError(partOfCommandLine, commands[i]);
    }

    /*for (int i = index; i < lineLength; i++) {
        mistakes[i] = '^';
        hasErrors = true;
    }

    if (hasErrors) {
        std::cerr << "Error - unexpected characters:" << std::endl << commandLine << std::endl;
        for (int i = 0; i < lineLength; i++) {
            std::cerr << mistakes[i];
        }
        std::cerr << std::endl;
    }*/

    return hasErrors;
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
    std::cout << argument << std::endl;
    if (!argument.empty() && commandLine[index] != '<' && commandLine[index] != '>' && index < commandLine.length()) {
        int firstQuote = commandLine.find('"', index);
        int secondQuote = commandLine.find('"', firstQuote + 1);

        if (firstQuote != std::string::npos && secondQuote != std::string::npos) {
            if (firstQuote > index || secondQuote + 1 < commandLine.length()) {
                for (int i = index; i < firstQuote; i++) {
                    if (!std::isspace(commandLine[i]) && commandLine[i] != '\t') {
                        mistakes[i] = '^';
                        hasErrors = true;
                    }
                }
                for (int i = secondQuote + 1; i < commandLine.length(); i++) {
                    if (commandLine[i] == '>' || commandLine[i] == '<') {
                        break;
                    }
                    if (!std::isspace(commandLine[i]) && commandLine[i] != '\t') {
                        mistakes[i] = '^';
                        hasErrors = true;
                    }
                }
            }
        }
        else {
            if (argument.find(' ') != std::string::npos) {
                for (int i = 0; i < argument.length(); i++) {
                    if (std::isspace(argument[i])) {
                        mistakes[index + i] = '^';
                        hasErrors = true;
                    }
                }
            }
        }
    }
    index += argument.length();
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

bool ErrorHandling::validateEcho(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        std::cerr << "The command does not take an option. Its format is: echo [argument]" << std::endl;
        return false;
    }
    return true;
}

bool ErrorHandling::validatePrompt(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        std::cerr << "The command does not take an option. Its format is: prompt argument" << std::endl;
        return false;
    }
    else if (parsedCommand.commandArg.empty()) {
        std::cerr << "The command takes an argument. Its format is: prompt argument" << std::endl;
        return false;
    }
    return true;
}

bool ErrorHandling::validateTime(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        std::cerr << "The command does not take an option. Its format is: time" << std::endl;
        return false;
    }
    else if (!parsedCommand.commandArg.empty()) {
        std::cerr << "The command does not take an argument. Its format is: time" << std::endl;
        return false;
    }
    return true;
}

bool ErrorHandling::validateDate(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        std::cerr << "The command does not take an option. Its format is: date" << std::endl;
        return false;
    }
    else if (!parsedCommand.commandArg.empty()) {
        std::cerr << "The command does not take an argument. Its format is: date" << std::endl;
        return false;
    }
    return true;
}

bool ErrorHandling::validateTouch(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        std::cerr << "The command does not take an option. Its format is: touch filename" << std::endl;
        return false;
    }
    else if (parsedCommand.commandArg.empty()) {
        std::cerr << "The command takes an argument. Its format is: touch argument" << std::endl;
        return false;
    }
    return true;
}

bool ErrorHandling::validateTruncate(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        std::cerr << "The command does not take an option. Its format is: truncate filename" << std::endl;
        return false;
    }
    else if (parsedCommand.commandArg.empty()) {
        std::cerr << "The command takes an argument. Its format is: truncate argument" << std::endl;
        return false;
    }
    return true;
}

bool ErrorHandling::validateRm(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        std::cerr << "The command does not take an option. Its format is: rm filename" << std::endl;
        return false;
    }
    else if (parsedCommand.commandArg.empty()) {
        std::cerr << "The command takes an argument. Its format is: rm argument" << std::endl;
        return false;
    }
    return true;
}

bool ErrorHandling::validateWc(Parser::ParsedCommand parsedCommand) {
    if (parsedCommand.commandOpt.empty()) {
        std::cerr << "The command takes an option. Its format is: wc -opt [argument]" << std::endl;
        return false;
    }
    return true;
}

bool ErrorHandling::validateTr(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        std::cerr << "The command does not take an option. Its format is: tr [argument] what [with]" << std::endl;
        return false;
    }
    return true;
}

bool ErrorHandling::validateHead(Parser::ParsedCommand parsedCommand) {
    if (parsedCommand.commandOpt.empty()) {
        std::cerr << "The command takes an option. Its format is: head -ncount [argument]" << std::endl;
        return false;
    }
    std::cout << parsedCommand.commandOpt << parsedCommand.commandArg << std::endl;
    return true;
}

bool ErrorHandling::validateBatch(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        std::cerr << "The command does not take an option. Its format is: batch [argument]" << std::endl;
        return false;
    }
    return true;
}


