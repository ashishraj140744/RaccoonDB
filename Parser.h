#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
using namespace std;
class Condition{
public:
    string column;
    string comparisonOperator;
    string value;
};
bool parseSelect(
    const vector<string>& tokens,
    vector<string>& selectedColumns,
    string& tableName,
    vector<Condition>& conditions,
    vector<string>& logicalOperators,
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