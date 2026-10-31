#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <string>
#include <vector>
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
#endif