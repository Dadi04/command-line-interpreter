#include "Command.h"
#include "Parser.h"
#include "CommandFactory.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>

// metoda koja odredjuje da li je argument string ili file
std::string Command::getArgumentType() {
	std::string input;
	if (!argument.empty()) {
		if (argument.front() == '"' && argument.back() == '"') {
			input = argument.substr(1, argument.size() - 2);
		}
		else {
			std::ifstream file(argument);
			if (file.is_open()) {
				std::string line;
				while (std::getline(file, line)) {
					input += line + '\n';
				}
				if (!input.empty() && input.back() == '\n') {
					input.pop_back();
				}
				file.close();
			}
			else {
				std::cerr << "Error: File \"" << argument << "\" does not exist." << std::endl;
				return "";
			}
		}
	}
	return input;
}

std::string Command::ifArgumentEmpty() {
	if (argument.empty()) {
		std::string line;
		while (true) {
			if (!std::getline(std::cin, line)) break;
			argument += line + '\n';
		}
	}

	return argument;
}

void Command::RedirectInput(std::string& input) {
	for (int i = 0; i < streams.size(); i++) {
		Redirection& stream = streams[i];
		if (stream.type == Redirection::Input) {
			std::ifstream inputFile;
			inputFile.open(stream.file);
			if (!inputFile.is_open()) {
				std::cerr << "Error: File \"" << stream.file << "\" does not exist." << std::endl;
				return;
			}
			
			std::stringstream buffer;
			buffer << inputFile.rdbuf();
			input = buffer.str();
			inputFile.close();
		}
	}
}

void Command::RedirectOutput(std::string input) {
	bool redirected = false;

	for (int i = 0; i < streams.size(); i++) {
		Redirection& stream = streams[i];
		if (stream.type == Redirection::Output || stream.type == Redirection::Append) {
			std::ofstream outputFile;
			if (stream.type == Redirection::Output) {
				outputFile.open(stream.file, std::ios::trunc);
			}
			else if (stream.type == Redirection::Append) {
				outputFile.open(stream.file, std::ios::app);
			}

			if (!outputFile.is_open()) {
				std::cerr << "Error: File \"" << stream.file << "\" does not exist." << std::endl;
				return;
			}
			outputFile << input;
			outputFile.close();
			redirected = true;
			break;
		}
	}
	if (!redirected) {
		std::cout << input << std::endl;
	}
}

void Echo::execute() {
	std::string input;
	if (argument.empty()) {
		input = ifArgumentEmpty();
	}
	else {
		input = getArgumentType();
		if (input.empty()) return;
	}

	RedirectInput(input);

	if (input.back() == '\n') {
		input.pop_back();
	}

	RedirectOutput(input);
}

void Prompt::execute() {
	this->promptSign = argument + " ";
}

std::string Prompt::promptSign = "$ ";
std::string Prompt::getPromptSign() {
	return promptSign;
}

void Time::execute() {
	time_t timestamp;
	time(&timestamp);
	struct tm currentTime;

	if (localtime_s(&currentTime, &timestamp) == 0) {
		char timeString[9];
		strftime(timeString, sizeof(timeString), "%H:%M:%S", &currentTime);

		RedirectOutput(timeString);
	}
}

void Date::execute() {
	time_t timestamp;
	time(&timestamp);
	struct tm currentDate;

	if (localtime_s(&currentDate, &timestamp) == 0) {
		char dateString[11];
		strftime(dateString, sizeof(dateString), "%d.%m.%Y", &currentDate);

		RedirectOutput(dateString);
	}
}

void Touch::execute() {
	std::ifstream file(argument);
	if (file) {
		std::cerr << "Error: File \"" << argument << "\" already exists." << std::endl;
	}
	else {
		std::ofstream newFile(argument);
		if (newFile) {
			newFile.close();
			std::cout << "File \"" << argument << "\" created successfully." << std::endl;
		}
		else {
			std::cerr << "Error: File \"" << argument << "\" could not be created." << std::endl;
		}
	}
}

void Truncate::execute() {
	std::ifstream file(argument);
	if (!file) {
		std::cerr << "Error: File \"" << argument << "\" does not exist." << std::endl;
		file.close();
	}
	else {
		std::ofstream file(argument, std::ios::trunc);
		if (file.is_open()) {
			std::cout << "Content in file \"" << argument << "\" deleted successfully." << std::endl;
			file.close();
		}
		else {
			std::cerr << "Error: Failed to open \"" << argument << "\"." << std::endl;
		}
	}
}

void Rm::execute() {
	std::ifstream file(argument);
	if (!file) {
		std::cerr << "Error: File \"" << argument << "\" does not exist." << std::endl;
		file.close();
	}
	else {
		file.close();
		int status = remove(argument.c_str());
		if (status == 0) {
			std::cout << "File \"" << argument << "\" deleted successfully." << std::endl;
		}
		else {
			std::cerr << "Error: Failed to delete \"" << argument << "\"." << std::endl;
		}
	}
}

void Wc::execute() {
	std::string input;
	if (argument.empty()) {
		input = ifArgumentEmpty();
	}
	else {
		input = getArgumentType();
		if (input.empty()) return;
	}

	RedirectInput(input);

	if (option == "-w") {
		int wordCount = 0;
		bool inWord = false;
		for (int i = 0; i < input.length(); i++) {
			if (std::isspace(input[i])) {
				wordCount++;
				inWord = false;
			}
			else {
				inWord = true;
			}
		}
		if (inWord) {
			wordCount++;
		}

		std::string wordCountString = std::to_string(wordCount);
		RedirectOutput(wordCountString);
	}
	else if (option == "-c") {
		std::string CharacterCountString = std::to_string(input.length());
		RedirectOutput(CharacterCountString);
	}
	else {
		std::cerr << "Error: Command wc must have either option -w or -c." << std::endl;
	}
}

void Tr::execute() {
	if (argument.empty()) {
		std::cerr << "Error: Command tr must have an argument" << std::endl;
		return;
	}
	else if (what.empty()) {
		std::cerr << "Error: Command tr must have a 'what' you want to replace with" << std::endl;
		return;
	}
	else {
		std::string input = getArgumentType();
		if (input.empty()) return;
		size_t counter = 0;
		while ((counter = input.find(what, counter)) != std::string::npos) {
			input.replace(counter, what.length(), with);
			counter += with.length();
		}
		RedirectInput(input);
		RedirectOutput(input);
	}
}

void Tr::parseArguments(std::string arg) {
	std::string argument, what, with;
	int i = 0;
	if (arg.empty()) {
		return;
	}

	if (i < arg.length() && arg[i] == '"') {
		i++;
		while (i < arg.length()) {
			if (arg[i] == '"') {
				i++;
				break;
			}
			argument += arg[i];
			i++;
		}
		if (!argument.empty()) {
			this->argument = '\"' + argument + '\"';
		}
		else {
			this->argument = "";
			return;
		}	
	}
	else if (i < arg.length()) {
		while (i < arg.length()) {
			if (arg[i] == '"') {
				break;
			}
			argument += arg[i];
			i++;
		}
		this->argument = argument;
	}
	else {
		return;
	}
	
	while (i < arg.length() && (std::isspace(arg[i]) || arg[i] == '\t')) {
		i++;
	}

	if (i < arg.length() && arg[i] == '"') {
		i++;
		while (i < arg.length()) {
			if (arg[i] == '"') {
				i++;
				break;
			}
			what += arg[i];
			i++;
		}
		this->what = what;
	}
	else {
		return;
	}

	while (i < arg.length() && (std::isspace(arg[i]) || arg[i] == '\t')) {
		i++;
	}

	if (i < arg.length() && arg[i] == '"') {
		i++;
		while (i < arg.length()) {
			if (arg[i] == '"') {
				break;
			}
			with += arg[i];
			i++;
		}
		this->with = with;
	}	

	// odraditi parsiranje AAAAAAAAAAAAAAA
}

void Head::execute() {
	std::string input;
	if (argument.empty()) {
		input = ifArgumentEmpty();
	}
	else {
		input = getArgumentType();
		if (input.empty()) return;
	}

	RedirectInput(input);

	if (!option.empty() && option.rfind("-n", 0) == 0) {
		int n = stoi(option.substr(2, option.length() - 2));
		if (n > 0 && n < 100000) {
			std::istringstream stream(input);
			std::string line;
			std::string output;
			for (int i = 0; i < n; i++) {
				if (std::getline(stream, line)) {
					output += line + "\n";
				}
				else {
					break;
				}
			}
			if (output.back() == '\n') {
				output.pop_back();
			}
			RedirectOutput(output);
		}
		else {
			std::cerr << "Error: n count is either smaller than 1 or bigger than 99999" << std::endl;
		}
	}
	else {
		std::cerr << "Error: Missing option -n<count>" << std::endl;
	}
}

void Batch::execute() {
	std::string input;
	if (argument.empty()) {
		input = ifArgumentEmpty();
	}
	else {
		input = getArgumentType();
		if (input.empty()) return;
	}

	int maxCommands = 1;
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '\n') {
			maxCommands++;
		}
	}

	std::string* commandsArray = new std::string[maxCommands];
	int commandCount = 0;

	std::string currentCommand;
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '\n') {
			if (commandCount < maxCommands) {
				commandsArray[commandCount++] = currentCommand;
				currentCommand.clear();
			}
			else {
				std::cerr << "Error: Command count exceeds allocated size." << std::endl;
				break;
			}
		}
		else {
			currentCommand += input[i];
		}
	}

	if (!currentCommand.empty() && commandCount < maxCommands) {
		commandsArray[commandCount++] = currentCommand;
	}

	for (int i = 0; i < commandCount; i++) {
		Parser commandParser;
		CommandFactory commandFactory;

		Parser::ParsedCommand parsedCommand = commandParser.parseCommand(commandsArray[i]);
		Command* command = commandFactory.createCommand(parsedCommand.commandName, parsedCommand.commandOpt, parsedCommand.commandArg, parsedCommand.streams);

		if (!command) {
			std::cerr << "Unknown command: \"" << parsedCommand.commandName << "\"" << std::endl;
			continue;
		}

		command->execute();
		delete command;
	}

	delete[] commandsArray;
}
