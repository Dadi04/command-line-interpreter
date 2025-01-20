#ifndef REDIRECTION_H
#define REDIRECTION_H

#include <string>

struct Redirection {
	enum StreamType {
		None, // no redirection
		Input, // '<'
		Output, // '>'
		Append, // '>>'
	};
	StreamType type;
	std::string file;
};

#endif