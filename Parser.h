#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
using namespace std;
bool parseSelect(const vector<string>& tokens, vector<string>& selectedColumns, string& tableName, string& errorMessage);
#endif