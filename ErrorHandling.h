#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

class ErrorHandling {
	bool isCorrectCommandName() {}
	bool isCorrectOption() {}
	bool isCorrectArgument() {}

};

class SemanticError : public ErrorHandling {
	
};

class OtherError : public ErrorHandling {

};

#endif