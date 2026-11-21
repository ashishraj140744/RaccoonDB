#include <iostream>
#include <string>
#include <editline/readline.h>
#include <cstdlib>
#include <cctype>
#include <iomanip>
#include <vector>
#include "Tokenizer.h"
#include "Parser.h"
#include "Database.h"
#include "Storage.h"
#include <filesystem>
using namespace std;
bool isKeyword(string word){
    string keywords[] = {
        "CREATE", "DATABASE", "TABLE", "SHOW", "DATABASES",
        "DROP", "DESCRIBE", "USE", "SORT", "BY", "ASC", "DESC",
        "SEARCH", "HELP", "STATS", "STATUS", "SELECT", "FROM",
        "WHERE", "ORDER", "AND", "OR", "INSERT", "INTO", "VALUES",
        "UPDATE", "SET", "DELETE", "EXIT", "QUIT"
    };
    int keywordCount = sizeof(keywords) / sizeof(keywords[0]);
    for(int i = 0; i < keywordCount; i++){
        if(word == keywords[i]){
            return true;
        }
    }
    return false;
}
void normalizeKeywords(vector<string>& tokens){
    for(int i = 0; i < tokens.size(); i++){
        if(tokens[i].length() >= 2 &&
           (tokens[i][0] == '\'' || tokens[i][0] == '"')){
            continue;
        }
        string upperWord = tokens[i];
        for(int j = 0; j < upperWord.length(); j++){
            upperWord[j] = toupper(
                static_cast<unsigned char>(upperWord[j])
            );
        }
        if(isKeyword(upperWord)){
            tokens[i] = upperWord;
        }
    }
}
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
    DatabaseManager manager;
    string storageError;
    if(!loadDatabaseManager(manager, "data", storageError)){
        if(filesystem::exists("data/databases.txt")){
            cout << "Storage error: " << storageError << endl;
            return 1;
        }
        Database defaultDatabase;
        defaultDatabase.name = "default";
        createSampleDatabase(defaultDatabase);
        manager.databases.push_back(defaultDatabase);
        manager.currentDatabaseIndex = 0;
        if(!saveDatabaseManager(manager, "data", storageError)){
            cout << "Storage error: " << storageError << endl;
        }
        else{
            cout << "Initial database created." << endl;
        }
    }
    else{
        cout << "Saved databases loaded successfully." << endl;
    }
    auto saveChanges = [&manager](){
        string errorMessage;
        if(!saveDatabaseManager(manager, "data", errorMessage)){
            cout << "Warning: " << errorMessage << endl;
        }
    };
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
            command += line + " ";
            if(hasStatementEnd(command)){
                break;
            }
        }
        add_history(command.c_str());
        vector<string> tokens = tokenize(command);
        if(tokens.empty()){
            continue;
        }
        normalizeKeywords(tokens);
        string commandType = tokens[0];
        Database& db = manager.databases[manager.currentDatabaseIndex];
        if(commandType == "CREATE" &&
            tokens.size() > 1 &&
            tokens[1] == "DATABASE"){
            string databaseName;
            string errorMessage;
            if(!parseCreateDatabase(
                tokens,
                databaseName,
                errorMessage
            )){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            if(createDatabase(
                manager,
                databaseName,
                errorMessage
            )){
                saveChanges();
                cout << errorMessage << endl;
            }
            else{
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        if(commandType == "SHOW" &&
            tokens.size() > 1 &&
            tokens[1] == "DATABASES"){
                string errorMessage;
                if(!parseShowDatabases(tokens, errorMessage)){
                    cout << "Parser error: " << errorMessage << endl;
                    continue;
                }
                cout << "Databases:" << endl;
                for(int i = 0; i < manager.databases.size(); i++){
                    cout << manager.databases[i].name << endl;
                }
                continue;
            }
        if(commandType == "DROP" &&
        tokens.size() > 1 &&
        tokens[1] == "DATABASE"){
            string databaseName;
            string errorMessage;
            if(!parseDropDatabase(tokens, databaseName, errorMessage)){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            if(dropDatabase(manager, databaseName, errorMessage)){
                saveChanges();
                cout << errorMessage << endl;
            }
            else{
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        if(commandType == "DROP" &&
        tokens.size() > 1 &&
        tokens[1] == "TABLE"){
            string tableName;
            string errorMessage;
            if(!parseDropTable(tokens, tableName, errorMessage)){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            if(dropTable(db, tableName, errorMessage)){
                saveChanges();
                cout << errorMessage << endl;
            }
            else{
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        if(commandType == "DESCRIBE" &&
        tokens.size() > 1 &&
        tokens[1] == "TABLE"){
            string tableName;
            string errorMessage;
            if(!parseDescribeTable(tokens, tableName, errorMessage)){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            Table* table = findTable(db, tableName);
            if(table == nullptr){
                cout << "Error: Table '" << tableName << "' does not exist." << endl;
                continue;
            }
            cout << "Table: " << table->name << endl;
            cout << endl;
            cout << left << setw(20) << "Column Name"
                << "Data Type" << endl;
            for(int i = 0; i < table->columns.size(); i++){
                cout << left << setw(20) << table->columns[i].name
                    << table->columns[i].type << endl;
            }
            continue;
        }
        if(commandType == "USE"){
            string databaseName;
            string errorMessage;
            if(!parseUseDatabase(tokens, databaseName, errorMessage)){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            if(useDatabase(manager, databaseName, errorMessage)){
                cout << errorMessage << endl;
            }
            else{
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        if(commandType == "SHOW" &&
            tokens.size() > 1 &&
            tokens[1] == "TABLES"){
            string errorMessage;
            if(!parseShowTables(tokens, errorMessage)){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            cout << "Tables:" << endl;
            if(db.tables.empty()){
                cout << "No tables found." << endl;
            }
            else{
                for(int i = 0; i < db.tables.size(); i++){
                    cout << db.tables[i].name << endl;
                }
            }
            continue;
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
                saveChanges();
                cout << errorMessage << endl;
            }
            else{
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        if(commandType == "SORT"){
            string tableName;
            string columnName;
            string direction;
            string errorMessage;
            if(!parseSort(
                tokens,
                tableName,
                columnName,
                direction,
                errorMessage
            )){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            if(executeSortTable(
                db,
                tableName,
                columnName,
                direction,
                errorMessage
            )){
                saveChanges();
                cout << errorMessage << endl;
            }
            else{
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        if(commandType == "SEARCH"){
            string tableName;
            string columnName;
            string searchValue;
            string errorMessage;
            if(!parseSearch(
                tokens,
                tableName,
                columnName,
                searchValue,
                errorMessage
            )){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            if(!executeSearch(
                db,
                tableName,
                columnName,
                searchValue,
                errorMessage
            )){
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        if(commandType == "HELP"){
            cout << "\n========== RACCOONDB GUIDE ==========\n";
            cout << "\nDATABASE COMMANDS:\n";
            cout << "CREATE DATABASE database_name;\n";
            cout << "SHOW DATABASES;\n";
            cout << "USE database_name;\n";
            cout << "DROP DATABASE database_name;\n";
            cout << "\nTABLE COMMANDS:\n";
            cout << "CREATE TABLE table_name (...);\n";
            cout << "SHOW TABLES;\n";
            cout << "DROP TABLE table_name;\n";
            cout << "DESCRIBE TABLE table_name;\n";
            cout << "\nDATA COMMANDS:\n";
            cout << "INSERT INTO table_name (...) VALUES (...);\n";
            cout << "SELECT * FROM table_name;\n";
            cout << "UPDATE table_name SET column = value WHERE condition;\n";
            cout << "DELETE FROM table_name WHERE condition;\n";
            cout << "\nSEARCH AND SORT COMMANDS:\n";
            cout << "SEARCH table_name BY column = value;\n";
            cout << "SORT table_name BY column ASC;\n";
            cout << "SORT table_name BY column DESC;\n";
            cout << "SELECT * FROM table_name ORDER BY column ASC;\n";
            cout << "\nOTHER COMMANDS:\n";
            cout << "HELP;\n";
            cout << "EXIT;\n";
            cout << "\n====================================\n";
            continue;
        }
        if(commandType == "STATS"){
            if(manager.currentDatabaseIndex < 0 ||
            manager.currentDatabaseIndex >= manager.databases.size()){
                cout << "Error: No database selected." << endl;
                continue;
            }
            Database& currentDatabase =
                manager.databases[manager.currentDatabaseIndex];
            cout << "\n========== DATABASE STATISTICS ==========\n";
            cout << "Database: " << currentDatabase.name << endl;
            cout << "Total Tables: " << currentDatabase.tables.size() << endl;
            int totalRows = 0;
            int totalColumns = 0;
            for(int i = 0; i < currentDatabase.tables.size(); i++){
                Table& table = currentDatabase.tables[i];
                cout << "\nTable: " << table.name << endl;
                cout << "Rows: " << table.rows.size() << endl;
                cout << "Columns: " << table.columns.size() << endl;
                cout << "Column Names: ";
                for(int j = 0; j < table.columns.size(); j++){
                    cout << table.columns[j].name;
                    if(j != table.columns.size() - 1){
                        cout << ", ";
                    }
                }
                cout << endl;
                totalRows += table.rows.size();
                totalColumns += table.columns.size();
            }
            cout << "\n---------- SUMMARY ----------\n";
            cout << "Total Records: " << totalRows << endl;
            cout << "Total Columns Across Tables: " << totalColumns << endl;
            cout << "==========================================\n";
            continue;
        }
        if(commandType == "SELECT"){
            vector<string> selectedColumns;
            string tableName;
            vector<Condition> conditions;
            vector<string> logicalOperators;
            bool hasOrderBy;
            string orderByColumn;
            string orderDirection;
            string errorMessage;
            if(!parseSelect(
                tokens,
                selectedColumns,
                tableName,
                conditions,
                logicalOperators,
                hasOrderBy,
                orderByColumn,
                orderDirection,
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
                hasOrderBy,
                orderByColumn,
                orderDirection,
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
                saveChanges();
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
                saveChanges();
                cout << errorMessage << endl;
            }
            else{
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        if(commandType == "UPDATE"){
            string tableName;
            vector<Assignment> assignments;
            vector<Condition> conditions;
            vector<string> logicalOperators;
            string errorMessage;
            if(!parseUpdate(
                tokens,
                tableName,
                assignments,
                conditions,
                logicalOperators,
                errorMessage
            )){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            if(executeUpdate(
                db,
                tableName,
                assignments,
                conditions,
                logicalOperators,
                errorMessage
            )){
                saveChanges();
                cout << errorMessage << endl;
            }
            else{
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        if(commandType == "DELETE"){
            string tableName;
            vector<Condition> conditions;
            vector<string> logicalOperators;
            string errorMessage;
            if(!parseDelete(
                tokens,
                tableName,
                conditions,
                logicalOperators,
                errorMessage
            )){
                cout << "Parser error: " << errorMessage << endl;
                continue;
            }
            if(executeDelete(
                db,
                tableName,
                conditions,
                logicalOperators,
                errorMessage
            )){
                saveChanges();
                cout << errorMessage << endl;
            }
            else{
                cout << "Error: " << errorMessage << endl;
            }
            continue;
        }
        if(commandType == "STATUS"){
            cout << "\n========== RACCOONDB STATUS ==========\n";
            cout << "Database Engine: RaccoonDB 1.0" << endl;
            if(manager.currentDatabaseIndex >= 0 &&
            manager.currentDatabaseIndex < manager.databases.size()){
                Database& currentDatabase =
                    manager.databases[manager.currentDatabaseIndex];
                cout << "Current Database: "
                    << currentDatabase.name << endl;
                cout << "Total Tables: "
                    << currentDatabase.tables.size() << endl;
                int totalRows = 0;
                for(int i = 0; i < currentDatabase.tables.size(); i++){
                    totalRows += currentDatabase.tables[i].rows.size();
                }
                cout << "Total Records: " << totalRows << endl;
            }
            else{
                cout << "Current Database: None" << endl;
            }
            cout << "Storage: CSV File Storage" << endl;
            cout << "======================================\n";
            continue;
        }
        cout << "Error: Unsupported command." << endl;
    }
    return 0;
}