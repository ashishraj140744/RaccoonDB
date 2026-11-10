#include <iostream>
#include <string>
#include <editline/readline.h>
#include <cstdlib>
#include <cctype>
#include <vector>
#include "Tokenizer.h"
#include "Parser.h"
#include "Database.h"
using namespace std;
bool hasStatementEnd(string command){
    char quote = '\0';
    for(int i = 0; i < command.length(); i++){
        char c = command[i];
        if(quote == '\0' && (c == '\'' || c == '"')){
            quote = c;
        }
        else if(quote == c){
            quote = '\0';
        }
        if(c == ';' && quote == '\0'){
            return true;
        }
    }
    return false;
}
bool isExitCommand(string command) {
    string upperCommand = "";
    for(int i = 0; i < command.length(); i++){
        if(!isspace(static_cast<unsigned char>(command[i]))){
            upperCommand += toupper(
                static_cast<unsigned char>(command[i])
            );
        }
    }
    return upperCommand == "EXIT" ||
           upperCommand == "EXIT;" ||
           upperCommand == "QUIT" ||
           upperCommand == "QUIT;";
}
int main(){
    cout << "           RaccoonDB 1.0" << endl;
    cout << "        Database Engine in C++" << endl;
    cout << "Type HELP; to see supported commands." << endl;
    cout << endl;
    Database db;
    createSampleDatabase(db);
    string command;
    string line;
    while (true) {
        command = "";
        while(true){
            string prompt;
            if(command.empty()){
                prompt = "raccoon> ";
            }
            else{
                prompt = "       -> ";
            }
            char* inputLine = readline(prompt.c_str());
            if(inputLine == nullptr){
                if(!command.empty()){
                    cout << "\nError: Incomplete command. Missing ';'." << endl;
                }
                return 0;
            }
            line = inputLine;
            free(inputLine);
            if(line.empty() && command.empty()){
                continue;
            }
            if(command.empty() && isExitCommand(line)){
                cout << "Goodbye!" << endl;
                return 0;
            }
            command += line + "\n";
            if(hasStatementEnd(command)){
                break;
            }
        }
        vector<string> tokens = tokenize(command);
        if(tokens.empty()){
            continue;
        }
        string commandType = tokens[0];
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
            vector<Condition> conditions;
            vector<string> logicalOperators;
            string errorMessage;
            if (!parseSelect(
                tokens,
                selectedColumns,
                tableName,
                conditions,
                logicalOperators,
                errorMessage
            )){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            Table* table = findTable(db, tableName);
            if(table == nullptr){
                cout << "Error: Table '" << tableName << "' does not exist." << endl;
                continue;
            }
            if(selectedColumns.size() == 1 &&
                selectedColumns[0] == "*"){
                selectedColumns.clear();
                for(int i = 0; i < table->columns.size(); i++){
                    selectedColumns.push_back(table->columns[i].name);
                }
            }
            if(!executeSelect(
                *table,
                selectedColumns,
                conditions,
                logicalOperators,
                errorMessage
            )){
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        if(commandType == "INSERT"){
            string tableName;
            vector<string> columnNames;
            vector<string> values;
            string errorMessage;
            if(!parseInsert(
                tokens,
                tableName,
                columnNames,
                values,
                errorMessage
            )){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            if(executeInsert(
                db,
                tableName,
                columnNames,
                values,
                errorMessage
            )){
                cout << errorMessage << endl;
            }
            else{
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        cout << "Error: Unsupported command." << endl;
    }
    return 0;
}