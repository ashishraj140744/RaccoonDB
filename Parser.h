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
#endif