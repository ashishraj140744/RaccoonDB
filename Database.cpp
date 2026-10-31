#include "Database.h"
void createSampleDatabase(Database& db){
    Table students;
    students.name = "students";
    Column col1;
    col1.name = "id";
    col1.type = "INT";
    Column col2;
    col2.name = "name";
    col2.type = "TEXT";
    Column col3;
    col3.name = "age";
    col3.type = "INT";
    Column col4;
    col4.name = "city";
    col4.type = "TEXT";
    students.columns.push_back(col1);
    students.columns.push_back(col2);
    students.columns.push_back(col3);
    students.columns.push_back(col4);
    Row row1;
    row1.values = {"1", "Ashish", "21", "Shimla"};
    Row row2;
    row2.values = {"2", "Rahul", "22", "Delhi"};
    Row row3;
    row3.values = {"3", "Priya", "20", "Mumbai"};
    Row row4;
    row4.values = {"4", "Aman", "19", "Chandigarh"};
    students.rows.push_back(row1);
    students.rows.push_back(row2);
    students.rows.push_back(row3);
    students.rows.push_back(row4);
    db.tables.push_back(students);
}
Table* findTable(Database& db, string tableName){
    for(int i = 0; i < db.tables.size(); i++){
        if(db.tables[i].name == tableName){
            return &db.tables[i];
        }
    }
    return nullptr;
}
void executeSelect(Table& table, vector<string> selectedColumns){
    vector<int> columnIndexes;
    for(int i = 0; i < selectedColumns.size(); i++){
        bool found = false;
        for(int j = 0; j < table.columns.size(); j++){
            if(selectedColumns[i] == table.columns[j].name){
                columnIndexes.push_back(j);
                found = true;
                break;
            }
        }
        if(!found){
            cout << "Error: Column '" << selectedColumns[i] << "' does not exist." << endl;
            return;
        }
    }
    for(int i = 0; i < columnIndexes.size(); i++){
        cout << table.columns[columnIndexes[i]].name;
        if(i != columnIndexes.size() - 1){
            cout << "\t";
        }
    }
    cout << endl;
    for(int i = 0; i < table.rows.size(); i++){
        for(int j = 0; j < columnIndexes.size(); j++){
            cout << table.rows[i].values[columnIndexes[j]];
            if(j != columnIndexes.size() - 1){
                cout << "\t";
            }
        }
        cout << endl;
    }
}