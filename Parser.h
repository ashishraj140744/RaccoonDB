#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
using namespace std;
bool parseSelect(const vector<string>& tokens, string& selectedColumn, string& tableName, string& errorMessage);
#endif