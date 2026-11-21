#include "Storage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include <exception>
using namespace std;
namespace fs = std::filesystem;
void writeCSVRow(ofstream& file, const vector<string>& values){
    for(int i = 0; i < values.size(); i++){
        if(i > 0){
            file << ",";
        }
        file << "\"";
        for(char ch : values[i]){
            if(ch == '"'){
                file << "\"\"";
            }
            else{
                file << ch;
            }
        }
        file << "\"";
    }
    file << "\n";
}
vector<string> readCSVRow(string line){
    vector<string> values;
    string value;
    bool insideQuotes = false;
    for(int i = 0; i < line.length(); i++){
        char ch = line[i];
        if(ch == '"'){
            if(insideQuotes &&
               i + 1 < line.length() &&
               line[i + 1] == '"'){
                value += '"';
                i++;
            }
            else{
                insideQuotes = !insideQuotes;
            }
        }
        else if(ch == ',' && !insideQuotes){
            values.push_back(value);
            value = "";
        }
        else{
            value += ch;
        }
    }
    values.push_back(value);
    return values;
}
bool saveDatabaseManager(
    const DatabaseManager& manager,
    const string& folderPath,
    string& errorMessage
){
    try{
        if(fs::exists(folderPath)){
            fs::remove_all(folderPath);
        }
        fs::create_directories(folderPath);
        ofstream databaseFile(
            folderPath + "/databases.txt"
        );
        if(!databaseFile.is_open()){
            errorMessage = "Could not create databases.txt.";
            return false;
        }
        databaseFile << manager.currentDatabaseIndex << "\n";
        databaseFile << manager.databases.size() << "\n";
        for(const Database& db : manager.databases){
            databaseFile << quoted(db.name) << "\n";
            string databasePath = folderPath + "/" + db.name;
            fs::create_directories(databasePath);
            for(const Table& table : db.tables){
                string filePath = databasePath + "/" + table.name + ".csv";
                ofstream tableFile(filePath);
                if(!tableFile.is_open()){
                    errorMessage = "Could not save table " + table.name;
                    return false;
                }
                vector<string> columnNames;
                vector<string> columnTypes;
                for(const Column& column : table.columns){
                    columnNames.push_back(column.name);
                    columnTypes.push_back(column.type);
                }
                writeCSVRow(tableFile, columnNames);
                writeCSVRow(tableFile, columnTypes);
                for(const Row& row : table.rows){
                    writeCSVRow(tableFile, row.values);
                }
            }
        }
        databaseFile.close();
        errorMessage = "Database saved successfully.";
        return true;
    }
    catch(const exception& e){
        errorMessage = "Storage error: " + string(e.what());
        return false;
    }
}
bool loadDatabaseManager(
    DatabaseManager& manager,
    const string& folderPath,
    string& errorMessage
){
    try{
        string metadataPath =
            folderPath + "/databases.txt";
        ifstream databaseFile(metadataPath);
        if(!databaseFile.is_open()){
            errorMessage = "No saved database found.";
            return false;
        }
        int currentIndex;
        int databaseCount;
        databaseFile >> currentIndex;
        databaseFile >> databaseCount;
        if(!databaseFile ||
           currentIndex < 0 ||
           databaseCount < 0){
            errorMessage = "Invalid database metadata.";
            return false;
        }
        vector<Database> loadedDatabases;
        for(int i = 0; i < databaseCount; i++){
            Database db;
            databaseFile >> quoted(db.name);
            if(!databaseFile){
                errorMessage = "Could not read database name.";
                return false;
            }
            string databasePath =
                folderPath + "/" + db.name;
            if(!fs::exists(databasePath)){
                errorMessage = "Database folder is missing.";
                return false;
            }
            for(const auto& entry : fs::directory_iterator(databasePath)){
                if(entry.path().extension() != ".csv"){
                    continue;
                }
                Table table;
                table.name = entry.path().stem().string();
                ifstream tableFile(entry.path());
                if(!tableFile.is_open()){
                    errorMessage = "Could not open table CSV file.";
                    return false;
                }
                string line;
                if(!getline(tableFile, line)){
                    errorMessage = "Missing column names.";
                    return false;
                }
                vector<string> columnNames = readCSVRow(line);
                if(!getline(tableFile, line)){
                    errorMessage = "Missing column datatypes.";
                    return false;
                }
                vector<string> columnTypes = readCSVRow(line);
                if(columnNames.size() != columnTypes.size()){
                    errorMessage = "Invalid table structure.";
                    return false;
                }
                for(int j = 0; j < columnNames.size(); j++){
                    Column column;
                    column.name = columnNames[j];
                    column.type = columnTypes[j];
                    table.columns.push_back(column);
                }
                while(getline(tableFile, line)){
                    vector<string> values = readCSVRow(line);
                    if(values.size() != table.columns.size()){
                        errorMessage = "Invalid record in " + table.name;
                        return false;
                    }
                    Row row;
                    row.values = values;
                    table.rows.push_back(row);
                }
                db.tables.push_back(table);
            }
            loadedDatabases.push_back(db);
        }
        databaseFile.close();
        if(currentIndex >= databaseCount){
            errorMessage = "Invalid selected database index.";
            return false;
        }
        manager.databases = loadedDatabases;
        manager.currentDatabaseIndex = currentIndex;
        errorMessage = "Database loaded successfully.";
        return true;
    }
    catch(const exception& e){
        errorMessage = "Storage error: " + string(e.what());
        return false;
    }
}