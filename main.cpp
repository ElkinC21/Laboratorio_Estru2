#include "CliArgs.hpp"
#include "FileManager.hpp"
#include <iostream>

int main(int argc, const char* argv[]) {
    CliArgs cli(argc, argv);

    if (cli.isGood() == false) {
        cli.printUsage();
        return 1;
    }

    FileManager manager("Students.dat", "Index.dat");

    if (cli.getCommand() == CliCommand::Add) {
        manager.addStudent(cli.getArgument());
    } 
    else if (cli.getCommand() == CliCommand::Delete) {
        manager.deleteStudent(cli.getArgument());
    } 
    else if (cli.getCommand() == CliCommand::Find) {
        manager.searchStudent(cli.getArgument());
    } 
    else if (cli.getCommand() == CliCommand::Update) {
        manager.updateStudent(cli.getArgument());
    } 
    else if (cli.getCommand() == CliCommand::CleanUp) {
        manager.cleanUp(); 
    }

    return 0;
}