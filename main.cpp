// Command
// Echo - echo [argument] - Prosledjuje sa ulaza na izlaz, nema opcije, ima opcioni argument tj moze i ne mora da pise ista na ulazu
// Time - time - U consolu ispisuje vreme, ne moze da ga izbaci u fajl, nema opcije, nema argument
// Date - date - U consolu ispisuje datum, ne moze da ga izbaci u fajl, nema opcije, nema argument
// Touch - touch argument - pravi fajl zadatak argumentom i sa praznim sadrzajem, ukoliko postoji izbacuje gresku i nema efekta komanda, nema opcije, ima argument
// Wc - wc -opt [argument] - broji reci ili sve znakove sa console i ispisuje u consolu, reci: sekvence odvojene belinama, belina: std::isspace = true, ima dve opcije -c i -w, ima opcioni argument

// Tek za januar
// Prompt, Truncate, Rm, Tr, Head, Batch 

// IOStream
// ConsoleIO
// FileIO

// Interpreter

// Tek za januar
// Pipeline
// Redirection
// '<' cita iz text fajla
// '>' brise sve iz text file, upisuje u text file, ako text file ne postoji, napravi ga
// '>>' upisuje u text file, ali na kraju


// ErrorHandling
// SemanticError
// OtherError

// Tek za januar
// LexicalError
// UnknownError
// SyntaxError

#include "Command.h"
#include "Parser.h"
#include <iostream>

const int MAX_INPUT_LENGHT = 512;

int main() {
	Parser commandParser;
	char input[MAX_INPUT_LENGHT];
	std::string prompt = "$ ";

	while (true) {
		std::cout << prompt;
		std::cin.getline(input, MAX_INPUT_LENGHT);

		if (input[0] == '\0') continue;

		Command* command = commandParser.parseCommand(input);
		if (command == nullptr) continue;

		command->execute();

		delete command;
	}
	return 0;
}