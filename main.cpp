#include "Interpreter.h"

int main() {
	Interpreter interpreter;

	interpreter.run();

	return 0;
}


// Problem 1 : Leksicka pravila 1
// Potentional fix in Interpreter.cpp
//#include <cstring>
//char input[MAX_INPUT_LENGTH + 1];
//std::cin.get(input, MAX_INPUT_LENGHT + 1);
//if (std::cin.gcount() == MAX_INPUT_LENGTH && std::cin.peek() != '\n') {
//	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//	std::cout << "Input exceeds the maximum length of " << MAX_INPUT_LENGTH << " characters. Excess characters ignored." << std::endl;
//}
//else {
//	if (std::cin.peek() == '\n') std::cin.ignore();
//}

// Problem 2 : Ulazni i izlazni tok komande 1 - Da li je potrebno za K2

// Problem 3 : Sta nam je bitno od errora, tipa da li echo -k "neki tekst" moze da dodje kao primer