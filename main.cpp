#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include "Tokenizer.h"
#include "Parser.h"
#include "Database.h"
using namespace std;
int main(){
    cout << "           RaccoonDB 1.0" << endl;
    cout << "        Database Engine in C++" << endl;
    cout << "Type HELP; to see supported commands." << endl;
    cout << endl;
    Database db;
    createSampleDatabase(db);
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
        bool hasWhere = false;
        string conditionColumn;
        string conditionOperator;
        string conditionValue;
        string errorMessage;
        if(!parseSelect(
            tokens, 
            selectedColumns, 
            tableName, 
            hasWhere,
            conditionColumn,
            conditionOperator,
            conditionValue, 
            errorMessage
        )){
            cout << "Parser error: " << errorMessage << endl;
            continue;
        }
        if(hasWhere){
            cout << "WHERE filtering is not implemented yet." << endl;
            continue;
        }
        Table* table = findTable(db, tableName);
        if(table == nullptr){
            cout << "Error: Table '" << tableName << "' does not exist." << endl;
            continue;
        }
        if(selectedColumns.size() == 1 && selectedColumns[0] == "*"){
            selectedColumns.clear();
            for(int i = 0; i < table->columns.size(); i++){
                selectedColumns.push_back(
                    table->columns[i].name
                );
            }
        }
        executeSelect(*table, selectedColumns);
    }
    return 0;
}