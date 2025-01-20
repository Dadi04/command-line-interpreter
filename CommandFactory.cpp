#include "CommandFactory.h"
#include "Command.h"
#include "Redirection.h"
#include <vector>

Command* CommandFactory::createCommand(std::string name, std::string opt, std::string arg, std::vector<Redirection> streams) {
    if (name == "echo") {
        return new Echo(arg, streams);
    }
    else if (name == "prompt") {
        return new Prompt(arg, streams);
    }
    else if (name == "time") {
        return new Time(streams);
    }
    else if (name == "date") {
        return new Date(streams);
    }
    else if (name == "touch") {
        return new Touch(arg, streams);
    }
    else if (name == "truncate") {
        return new Truncate(arg, streams);
    }
    else if (name == "rm") {
        return new Rm(arg, streams);
    }
    else if (name == "wc") {
        return new Wc(opt, arg, streams);
    } 
    else if (name == "tr") {
        return new Tr(arg, streams);
    }
    else if (name == "head") {
        return new Head(opt, arg, streams);
    }
    else if (name == "batch") {
        return new Batch(arg, streams);
    }
    else {
        return nullptr;
    }
}