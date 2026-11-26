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
    vector<int> indexedColumns;
    vector<int> bstIndexedColumns;
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
    const vector<vector<string>>& rows,
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
bool executeSortTable(
    Database& db,
    string tableName,
    string columnName,
    string direction,
    string& errorMessage
);
vector<int> linearSearch(
    const Table& table,
    int columnIndex,
    string searchValue
);
vector<int> linearSearchCondition(
    const Table& table,
    int columnIndex,
    string searchValue,
    string comparisonOperator,
    bool numeric
);
bool createIndex(
    Database& db,
    string tableName,
    string columnName,
    string& errorMessage
);
void buildHashIndex(string databaseName, Table& table, int columnIndex);
bool searchHashIndex(
    string databaseName,
    const Table& table,
    int columnIndex,
    string searchValue,
    vector<int>& result
);
void rebuildHashIndexes(string databaseName, Table& table);
bool executeSearch(
    Database& db,
    string tableName,
    string columnName,
    string comparisonOperator,
    string searchValue,
    string& errorMessage
);
bool createBSTIndex(
    Database& db,
    string tableName,
    string columnName,
    string& errorMessage
);
void buildBSTIndex(
    string databaseName,
    Table& table,
    int columnIndex
);
bool searchBSTIndex(
    string databaseName,
    const Table& table,
    int columnIndex,
    int searchValue,
    string comparisonOperator,
    vector<int>& results
);
#endif