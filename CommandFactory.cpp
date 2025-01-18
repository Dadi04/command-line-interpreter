#include "CommandFactory.h"
#include "Command.h"

Command* CommandFactory::createCommand(std::string name, std::string opt, std::string arg) {
    if (name == "echo") {
        return new Echo(arg);
    }
    else if (name == "prompt") {
        return new Prompt(arg);
    }
    else if (name == "time") {
        return new Time();
    }
    else if (name == "date") {
        return new Date();
    }
    else if (name == "touch") {
        return new Touch(arg);
    }
    else if (name == "truncate") {
        return new Truncate(arg);
    }
    else if (name == "rm") {
        return new Rm(arg);
    }
    else if (name == "wc") {
        return new Wc(opt, arg);
    } 
    //else if (name == "tr") {
    //    return new Tr(arg);
    //}
    else if (name == "head") {
        return new Head(opt, arg);
    }
    else if (name == "batch") {
        return new Batch(arg);
    }
    else {
        return nullptr;
    }
}