#ifndef CLIARGS_HPP
#define CLIARGS_HPP

#include <string>

enum class CliCommand {
    Add,
    Delete,
    Find,
    Update,
    CleanUp,
    None
};

class CliArgs {
private:
    std::string program;
    CliCommand command;
    std::string argument; 
    bool good;

public:
    CliArgs(int argc, const char* argv[]);
    
    bool isGood() const;
    CliCommand getCommand() const;
    std::string getArgument() const;
    void printUsage() const;

private:
    void parse(int argc, const char* argv[]);
};

#endif