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
};
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
#endif