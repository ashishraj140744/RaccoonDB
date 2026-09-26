#include "CommandLine.h"
#include <iostream>
#include <string>
using namespace std;
void CommandLine::start(){
    cout << "        RaccoonDB 1.0\n";
    string command;
    while (true) {
        cout << "\nraccoon> ";
        getline(cin, command);
        if (command == "EXIT;" || command == "exit;") {
            cout << "Goodbye.\n";
            break;
        }
        cout << "You entered: " << command << "\n";
    }
}