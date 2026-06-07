#include "CliArgs.hpp"
#include <iostream>

CliArgs::CliArgs(int argc, const char* argv[]) {
    this->program = "";
    this->command = CliCommand::None;
    this->argument = "";
    this->good = true;
    this->parse(argc, argv);
}

bool CliArgs::isGood() const {
    return this->good;
}

CliCommand CliArgs::getCommand() const {
    return this->command;
}

std::string CliArgs::getArgument() const {
    return this->argument;
}

void CliArgs::printUsage() const {
    std::cout << "Usage: " << this->program << " <command> [argument]\n\n";
    std::cout << "Available commands:\n";
    std::cout << "  --add_student <json_file>\n";
    std::cout << "  --delete_student <account_number>\n";
    std::cout << "  --find_student <account_number>\n";
    std::cout << "  --update_student <json_file>\n";
    std::cout << "  --clean-up\n";
}

void CliArgs::parse(int argc, const char* argv[]) {
    this->program = argv[0];

    if (argc < 2) {
        this->good = false;
        return;
    }

    std::string cmd = argv[1];

    if (cmd == "--clean-up") {
        this->command = CliCommand::CleanUp;
        if (argc != 2) {
            this->good = false;
        }
    } 
    else if (cmd == "--add_student") {
        this->command = CliCommand::Add;
        if (argc == 3) {
            this->argument = argv[2];
        } else {
            this->good = false;
        }
    } 
    else if (cmd == "--delete_student") {
        this->command = CliCommand::Delete;
        if (argc == 3) {
            this->argument = argv[2];
        } else {
            this->good = false;
        }
    } 
    else if (cmd == "--find_student") {
        this->command = CliCommand::Find;
        if (argc == 3) {
            this->argument = argv[2];
        } else {
            this->good = false;
        }
    } 
    else if (cmd == "--update_student") {
        this->command = CliCommand::Update;
        if (argc == 3) {
            this->argument = argv[2];
        } else {
            this->good = false;
        }
    } 
    else {
        this->good = false;
    }
}