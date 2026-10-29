#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include "Tokenizer.h"
#include "Parser.h"
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
        for(int i = 0; i < tokens.size(); i++){
            cout << "[" << tokens[i] << "] ";
        }
        cout << endl;
        vector<string> selectedColumns;
        string tableName;
        string errorMessage;
        if(parseSelect(tokens, selectedColumns, tableName, errorMessage)){
            cout << errorMessage << endl;
            cout << "Selected column: ";
            for(int i = 0; i < selectedColumns.size(); i++) {
                cout << selectedColumns[i];
                if(i < selectedColumns.size() - 1){
                    cout << ", ";
                }
            }
            cout << endl;
            cout << "Table name: " << tableName << endl;
        }
        else{
            cout << "Parser error: " << errorMessage << endl;
        }
    }
    return 0;
}