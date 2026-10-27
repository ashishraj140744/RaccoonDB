#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include "Tokenizer.h"
using namespace std;
int main(){
    cout << "           RaccoonDB 1.0" << endl;
    cout << "        Database Engine in C++" << endl;
    cout << "Type HELP; to see supported commands." << endl;
    cout << endl;
    string command;
    while(true){
        cout << "raccoon> ";
        if(!getline(cin, command)){
            break;
        }
        if(command.empty()){
            continue;
        }
        string upperCommand = command;
        for(int i = 0; i < upperCommand.length(); i++){
            upperCommand[i] = toupper(upperCommand[i]);
        }
        if(upperCommand == "EXIT" || upperCommand == "EXIT;" || upperCommand == "QUIT" || upperCommand == "QUIT;"){
            cout << "Goodbye!" << endl;
            break;
        }
        vector<string> tokens = tokenize(command);
        cout << "Tokens: ";
        for (size_t i = 0; i < tokens.size(); i++) {
            cout << "[" << tokens[i] << "] ";
        }
        cout << endl;
    }
    return 0;
}