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
        if(tokens.empty()){
            continue;
        }
        string commandType = tokens[0];
        cout << "Tokens: ";
        for (int i = 0; i < commandType.length(); i++) {
            commandType[i] = toupper(commandType[i]);
        }
        if(commandType == "CREATE"){
            string tableName;
            vector<string> columnNames;
            vector<string> columnTypes;
            string errorMessage;
            if(!parseCreateTable(
                tokens,
                tableName,
                columnNames,
                columnTypes,
                errorMessage
            )){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            if(createTable(
                db,
                tableName,
                columnNames,
                columnTypes,
                errorMessage
            )){
                cout << errorMessage << endl;
            }
            else{
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        if(commandType == "SELECT"){
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
            continue;
        }
        cout << "Error: Unsupported command." << endl;
    }
    return 0;
}