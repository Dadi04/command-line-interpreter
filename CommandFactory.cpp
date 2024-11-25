#include "CommandFactory.h"

Command* CommandFactory::createCommand(std::string name, std::string opt, std::string arg) {
    if (name == "echo") {
        return new Echo(arg);
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
    else if (name == "wc") {
        return new Wc(opt, arg);
    } 
    else {
        return nullptr;
    }
}