#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
using namespace std;
bool parseSelect( 
    const vector<string>& tokens, 
    vector<string>& selectedColumns, 
    string& tableName, 
    bool& hasWhere,
    string& conditionColumn,
    string& conditionOperator,
    string& conditionValue, 
    string& errorMessage
);
bool parseCreateTable(
    const vector<string>& tokens, 
    string& tableName,
    vector<string>& columnNames,
    vector<string>& columnTypes,
    string& errorMessage
);
bool parseInsert(
    const vector<string>& tokens,
    string& tableName,
    vector<string>& columnNames,
    vector<string>& values,
    string& errorMessage
);
#endif