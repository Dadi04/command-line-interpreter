#include "ErrorHandling.h"
#include "Command.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <regex>

bool ErrorHandling::validateCommand(Parser::ParsedCommand parsedCommand) {
    try {
        auto it = validators.find(parsedCommand.commandName);
        if (it == validators.end()) {
            throw std::runtime_error("Unknown command: " + parsedCommand.commandName);
        }
        it->second(parsedCommand);
    }
    catch (std::exception e) {
        std::cerr << "Error - " << e.what() << std::endl;
        return false;
    }
    return true;
}

void ErrorHandling::validateEcho(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        throw std::runtime_error("The command does not take an option. Its format is: echo [argument]");
    }
}

void ErrorHandling::validatePrompt(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        throw std::runtime_error("The command does not take an option. Its format is: prompt argument");
    }

    if (parsedCommand.commandArg.empty()) {
        throw std::runtime_error("The command takes an argument. Its format is: prompt argument");
    }

    if (parsedCommand.streams.size() != 0) {
        throw std::runtime_error("The command does not take a redirection");
    }
}

void ErrorHandling::validateTime(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        throw std::runtime_error("The command does not take an option. Its format is: time");
    }
    
    if (std::any_of(parsedCommand.streams.begin(), parsedCommand.streams.end(), [](Redirection stream) { return stream.type == Redirection::Input; })) {
        throw std::runtime_error("The command does not take an input redirection");
    }

    if (!parsedCommand.commandArg.empty()) {
        throw std::runtime_error("The command does not take an argument. Its format is: time");
    }
    
}

void ErrorHandling::validateDate(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        throw std::runtime_error("The command does not take an option. Its format is: date");
    }
    
    if (std::any_of(parsedCommand.streams.begin(), parsedCommand.streams.end(), [](Redirection stream) { return stream.type == Redirection::Input; })) {
        throw std::runtime_error("The command does not take an input redirection");
    }

    if (!parsedCommand.commandArg.empty()) {
        throw std::runtime_error("The command does not take an argument. Its format is: date");
    }
}

void ErrorHandling::validateTouch(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        throw std::runtime_error("The command does not take an option. Its format is: touch filename");
    }

    if (parsedCommand.commandArg.empty()) {
        throw std::runtime_error("The command takes an argument. Its format is: touch argument");
    }

    if (std::any_of(parsedCommand.streams.begin(), parsedCommand.streams.end(), [](Redirection stream) { return (stream.type == Redirection::Output || stream.type == Redirection::Append); })) {
        throw std::runtime_error("The command does not take an output redirection");
    }
}

void ErrorHandling::validateTruncate(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        throw std::runtime_error("The command does not take an option. Its format is: truncate filename");
    }

    if (parsedCommand.commandArg.empty()) {
        throw std::runtime_error("The command takes an argument. Its format is: truncate argument");
    }

    if (std::any_of(parsedCommand.streams.begin(), parsedCommand.streams.end(), [](Redirection stream) { return (stream.type == Redirection::Output || stream.type == Redirection::Append); })) {
        throw std::runtime_error("The command does not take an output redirection");
    }
}

void ErrorHandling::validateRm(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        throw std::runtime_error("The command does not take an option. Its format is: rm filename");
    }

    if (parsedCommand.commandArg.empty()) {
        throw std::runtime_error("The command takes an argument. Its format is: rm argument");
    }

    if (std::any_of(parsedCommand.streams.begin(), parsedCommand.streams.end(), [](Redirection stream) { return (stream.type == Redirection::Output || stream.type == Redirection::Append); })) {
        throw std::runtime_error("The command does not take an output redirection");
    }
}

void ErrorHandling::validateWc(Parser::ParsedCommand parsedCommand) {
    if (parsedCommand.commandOpt.empty()) {
        throw std::runtime_error("The command takes an option. Its format is: wc -opt [argument]");
    }
}

void ErrorHandling::validateTr(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        throw std::runtime_error("The command does not take an option. Its format is: tr [argument] what [with]");
    }
}

void ErrorHandling::validateHead(Parser::ParsedCommand parsedCommand) {
    if (parsedCommand.commandOpt.empty()) {
        throw std::runtime_error("The command takes an option. Its format is: head -ncount [argument]");
    }
}

void ErrorHandling::validateBatch(Parser::ParsedCommand parsedCommand) {
    if (!parsedCommand.commandOpt.empty()) {
        throw std::runtime_error("The command does not take an option. Its format is: batch [argument]");
    }
}

bool ErrorHandling::catchErrors(std::string commandLine, Parser::ParsedCommand command) {
    int lineLength = commandLine.length();
    char* mistakes = new char[lineLength + 1];
    for (int i = 0; i < lineLength; i++) {
        mistakes[i] = ' ';
    }

    int index = 0;
    try {
        skipWhiteSpace(commandLine, index);

        validateCommandName(commandLine, command.commandName, mistakes, index);

        skipWhiteSpace(commandLine, index);

        validateCommandOption(commandLine, command.commandOpt, mistakes, index);

        skipWhiteSpace(commandLine, index);

        if (command.commandName == "tr") {
            validateCommandArgumentTr(commandLine, command.commandArg, mistakes, index);
        }
        else if (command.commandName == "batch") {
            validateCommandArgumentBatch(commandLine, command.commandArg, mistakes, index);
        }
        else {
            validateCommandArgument(commandLine, command.commandArg, mistakes, index);
        }

        skipWhiteSpace(commandLine, index);

        validateStreams(commandLine, command.streams, mistakes, index);

        for (int i = index; i < lineLength; i++) {
            mistakes[i] = '^';
            throw std::runtime_error("Unexpected characters at the end of the command");
        }
        if (!command.commandArg.empty() && 
            (std::any_of(command.streams.begin(), command.streams.end(), [&command](Redirection stream) { return stream.file != command.commandArg; })) && 
            (std::any_of(command.streams.begin(), command.streams.end(), [](Redirection stream) { return stream.type == Redirection::Input; }))) {
            throw std::runtime_error("The command can only take one input redirection");
        }
    }
    catch (std::exception e) {
        std::cerr << "Error - " << e.what();

        if (!std::all_of(mistakes, mistakes + lineLength, [](char c) { return c == ' '; })) {
            std::cerr << ":" << std::endl << commandLine << std::endl;
            for (int i = 0; i < lineLength; i++) {
                std::cerr << mistakes[i];
            }
        }

        std::cerr << std::endl;
        delete[] mistakes;
        return true;
    }

    delete[] mistakes;
    return false;
}

void ErrorHandling::skipWhiteSpace(std::string commandLine, int& index) {
    while (index < commandLine.length() && (std::isspace(commandLine[index]) || commandLine[index] == '\t')) {
        index++;
    }
}

void ErrorHandling::validateCommandName(std::string commandLine, std::string name, char* mistakes, int& index) {
    for (int i = 0; i < name.length(); i++) {
        if (index < commandLine.length() && !std::isalpha(name[i])) {
            mistakes[index] = '^';
            throw std::runtime_error("Invalid command name");
        }
        index++;
    }
}

void ErrorHandling::validateCommandOption(std::string commandLine, std::string option, char* mistakes, int& index) {
    if (!option.empty()) {
        if (index < commandLine.length() && commandLine[index] == '-') {
            index++;
            for (int i = 1; i < option.length(); i++) {
                if (index < commandLine.length() && !std::isalpha(option[i]) && !std::isdigit(option[i])) {
                    mistakes[index] = '^';
                    throw std::runtime_error("Invalid character in command option");
                }
                index++;
            }
        }
        else if (index < commandLine.length()) {
            mistakes[index] = '^';
            throw std::runtime_error("Missing dash for command option");
            index++;
        }
    }
}

void ErrorHandling::validateCommandArgument(std::string commandLine, std::string argument, char* mistakes, int& index) {
    if (!argument.empty() && commandLine[index] != '<' && commandLine[index] != '>' && index < commandLine.length()) {
        int firstQuote = commandLine.find('"', index);
        int secondQuote = commandLine.find('"', firstQuote + 1);

        if (firstQuote != std::string::npos && secondQuote != std::string::npos) {
            if (firstQuote > index || secondQuote + 1 < commandLine.length()) {
                for (int i = index; i < firstQuote; i++) {
                    if (!std::isspace(commandLine[i]) && commandLine[i] != '\t') {
                        mistakes[i] = '^';
                        throw std::runtime_error("Unexpected character before quoted argument");
                    }
                }
                for (int i = secondQuote + 1; i < commandLine.length(); i++) {
                    if (commandLine[i] == '>' || commandLine[i] == '<' || commandLine[i] == '|') {
                        break;
                    }
                    if (!std::isspace(commandLine[i]) && commandLine[i] != '\t') {
                        mistakes[i] = '^';
                        throw std::runtime_error("Unexpected character after quoted argument");
                    }
                }
            }
        }
        else {
            if (argument.find(' ') != std::string::npos) {
                for (int i = 0; i < argument.length(); i++) {
                    if (std::isspace(argument[i])) {
                        mistakes[index + i] = '^';
                        throw std::runtime_error("Spaces are not allowed in the filename");
                    }
                }
            }
        }
        index += argument.length();
    }
}

void ErrorHandling::validateCommandArgumentTr(std::string commandLine, std::string argument, char* mistakes, int& index) {
    if (commandLine[index] == '"') {
        index++;
        for (int i = 1; i < argument.length(); i++) {
            if (argument[i] != '"') {
                index++;
            }
            else {
                index++;
                break;
            }
        }
    }
    else if (std::isalnum(commandLine[index])) {
        std::string filename;
        int tempIndex = -1, quote = commandLine.find('"', index), firstCharOfArg = index, lastCharOfArg;
        for (int i = 0; i < argument.length(); i++) {
            if (std::isspace(argument[i])) {
                tempIndex = index;
                index++;
                break;
            }
            filename += argument[i];
            index++;
        }
        lastCharOfArg = index;
        if (commandLine[index] != '"') {
            mistakes[tempIndex] = '^';
            throw std::runtime_error("Unexpected character in argument");
        } 
        if (quote > firstCharOfArg && quote < lastCharOfArg) {
            mistakes[firstCharOfArg] = '^';
            throw std::runtime_error("Unexpected character in argument");
        }
    }
    else {
        mistakes[index] = '^';
        throw std::runtime_error("Unexpected character in argument");
    }

    skipWhiteSpace(commandLine, index);
    for (int i = 0; i < 2; i++) {
        if (commandLine[index] == '"' && index < commandLine.length()) {
            index++;
            for (int j = 1; j < argument.length(); j++) {
                if (commandLine[index] != '"') {
                    index++;
                }
                else {
                    index++;
                    break;
                }
            }
        }
        else if (index < commandLine.length()) {
            mistakes[index] = '^';
            throw std::runtime_error("Unexpected character");
        }
        skipWhiteSpace(commandLine, index);
    }
}

void ErrorHandling::validateCommandArgumentBatch(std::string commandLine, std::string argument, char* mistakes, int& index) {
    int firstQuote = argument.find('"');
    int lastQuote = argument.rfind('"');

    if (firstQuote != std::string::npos) {
        for (int i = 0; i < firstQuote; i++) {
            if (!std::isspace(argument[i]) && argument[i] != '\t') {
                mistakes[index + i] = '^';
                throw std::runtime_error("Unexpected character before quoted argument");
            }
        }

        for (int i = lastQuote + 1; i < argument.size(); i++) {
            if (!std::isspace(argument[i]) && argument[i] != '\t') {
                mistakes[index + i] = '^';
                throw std::runtime_error("Unexpected character after quoted argument");
            }
        }
    }
    else {
        if (argument.find(' ') != std::string::npos) {
            for (int i = 0; i < argument.size(); i++) {
                if (argument[i] == ' ') {
                    mistakes[index + i] = '^';
                    throw std::runtime_error("Spaces are not allowed in the filename");
                }
            }
        }
    }

    index += argument.size();
}

void ErrorHandling::validateStreams(std::string commandLine, std::vector<Redirection> streams, char* mistakes, int& index) {
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
                        throw std::runtime_error("Spaces are not allowed in the filename");
                    }
                }
                index += fileName.length();
            }
            else {
                break;
            }
        }
    }
}

// logicke greske tipa ima vise argumenata u komandi, vise izlaznih redirekcija/argumenata itd
bool ErrorHandling::catchPipeErrors(std::string commandLine, std::vector<Parser::ParsedCommand> commands) {
    int lineLength = commandLine.length();
    char* mistakes = new char[lineLength + 1];
    for (int i = 0; i < lineLength; i++) {
        mistakes[i] = ' ';
    }

    int index = 0;
    try {
        for (int i = 0; i < commands.size(); i++) {
            if (i == 0) {
                if (std::any_of(commands[i].streams.begin(), commands[i].streams.end(), [](Redirection stream) { return (stream.type == Redirection::Output || stream.type == Redirection::Append); })) {
                    throw std::runtime_error("First command in the pipeline does not accept output redirection");
                }
            }
            else if (i == commands.size() - 1) {
                if (!commands[i].commandArg.empty() || std::any_of(commands[i].streams.begin(), commands[i].streams.end(), [](Redirection stream) { return stream.type == Redirection::Input; })) {
                    throw std::runtime_error("Last command in the pipeline does not accept argument or input redirection");
                }
            }
            else {
                if (!commands[i].commandArg.empty() || !commands[i].streams.empty()) {
                    throw std::runtime_error("Commands between first and last in the pipeline do not accept argument or redirections");
                }
            }

            skipWhiteSpace(commandLine, index);

            validateCommandName(commandLine, commands[i].commandName, mistakes, index);

            skipWhiteSpace(commandLine, index);

            validateCommandOption(commandLine, commands[i].commandOpt, mistakes, index);

            skipWhiteSpace(commandLine, index);

            if (commands[i].commandName.find("tr") != std::string::npos) {
                validateCommandArgumentTr(commandLine, commands[i].commandArg, mistakes, index);
            }
            else if (commands[i].commandName.find("batch") != std::string::npos) {
                validateCommandArgumentBatch(commandLine, commands[i].commandArg, mistakes, index);
            }
            else {
                validateCommandArgument(commandLine, commands[i].commandArg, mistakes, index);
            }

            skipWhiteSpace(commandLine, index);

            validateStreams(commandLine, commands[i].streams, mistakes, index);

            skipWhiteSpace(commandLine, index);

            index++;
        }
    }
    catch (std::exception e) {
        std::cerr << "Error - " << e.what();

        if (!std::all_of(mistakes, mistakes + lineLength, [](char c) { return c == ' '; })) {
            std::cerr << ":" << std::endl << commandLine << std::endl;
            for (int i = 0; i < lineLength; i++) {
                std::cerr << mistakes[i];
            }
        }

        std::cerr << std::endl;
        delete[] mistakes;
        return true;
    }

    delete[] mistakes;
    return false;
}



