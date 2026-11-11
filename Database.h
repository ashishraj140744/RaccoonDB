#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <string>
#include <vector>
#include "Parser.h"
using namespace std;
class Column{
public:
    string name;
    string type;
};
class Row{
public:
    vector<string> values;
};
class Table{
public:
    string name;
    vector<Column> columns;
    vector<Row> rows;
};
class Database{
public:
    vector<Table> tables;
    string name;
};
class DatabaseManager{
public:
    vector<Database> databases;
    int currentDatabaseIndex;
    DatabaseManager(){
        currentDatabaseIndex = -1;
    }
};
bool createDatabase(
    DatabaseManager& manager,
    string databaseName,
    string& errorMessage
);
void createSampleDatabase(Database& db);
Table* findTable(Database& db, string tableName);
bool executeSelect(
    Table& table,
    vector<string> selectedColumns,
    const vector<Condition>& conditions,
    const vector<string>& logicalOperators,
    bool hasOrderBy,
    string orderByColumn,
    string orderDirection,
    string& errorMessage
);
bool createTable(
    Database& db,
    string tableName,
    const vector<string>& columnNames,
    const vector<string>& columnTypes,
    string& errorMessage
);
bool executeInsert(
    Database& db,
    string tableName,
    const vector<string>& columnNames,
    const vector<string>& values,
    string& errorMessage
);
bool executeUpdate(
    Database& db,
    string tableName,
    const vector<Assignment>& assignments,
    const vector<Condition>& conditions,
    const vector<string>& logicalOperators,
    string& errorMessage
);
bool executeDelete(
    Database& db,
    string tableName,
    const vector<Condition>& conditions,
    const vector<string>& logicalOperators,
    string& errorMessage
);
bool useDatabase(
    DatabaseManager& manager,
    string databaseName,
    string& errorMessage
);
bool dropDatabase(
    DatabaseManager& manager,
    string databaseName,
    string& errorMessage
);
bool dropTable(
    Database& db,
    string tableName,
    string& errorMessage
);
#endif