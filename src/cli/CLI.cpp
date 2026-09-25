#include "CLI.h"
#include <iostream>
#include <string>
using namespace std;
void CLI::start() {
    cout << "================================\n";
    cout << "        RaccoonDB 1.0\n";
    cout << "================================\n";
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