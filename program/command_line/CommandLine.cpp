#include "CommandLine.h"
#include "../tokenizer/Tokenizer.h"
#include <iostream>
#include <string>
using namespace std;
void CommandLine::start() {
    cout << "================================\n";
    cout << "        RaccoonDB 1.0\n";
    cout << "================================\n";
    string command;
    Tokenizer tokenizer;
    while (true) {
        cout << "\nraccoon> ";
        getline(cin, command);
        if (command == "EXIT;" || command == "exit;") {
            cout << "Goodbye.\n";
            break;
        }
        try {
            vector<Token> tokens =
                tokenizer.tokenize(command);
            for (const Token& token : tokens) {
                cout
                    << "["
                    << token.value
                    << "]\n";
            }
        }
        catch (const exception& error) {
            cout
                << "Tokenizer error: "
                << error.what()
                << "\n";
        }
    }
}