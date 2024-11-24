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
#include "Interpreter.h"
#include <iostream>

int main() {
	//Date dateCommand;
	//dateCommand.execute();

	//Time timeCommand;
	//timeCommand.execute();

	//Echo echoCommand("Hello, World!");
	//echoCommand.execute();

	//Touch touchCommand("D:\\Documents\\David\\ETF\\text.txt");
	//touchCommand.execute();

	//Wc wcCommand1("-w", "Lorem ipsum dolor sit amet");
	//wcCommand1.execute();

	//Wc wcCommand2("-c", "Lorem ipsum dolor sit amet");
	//wcCommand2.execute();

	Interpreter interpreter;

	interpreter.run();

	return 0;
}