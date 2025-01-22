#ifndef REDIRECTION_H
#define REDIRECTION_H

#include <string>
#include <sstream>

struct Redirection {
	enum StreamType {
		None, // no redirection
		Input, // '<'
		Output, // '>'
		Append, // '>>'
	};
	StreamType type;
	std::string file;
	std::shared_ptr<std::stringstream> memoryStream;

	bool redirectInput(std::string& input);
	bool redirectOutput(std::string output);
};

#endif