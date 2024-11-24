#include "CommandFactory.h"
#include "Command.h"

Command* CommandFactory::createCommand(const std::string& name, const std::string& opt, const std::string& arg) {
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