#include "Database.h"
using namespace std;
vector<int> linearSearch(
    const Table& table,
    int columnIndex,
    string searchValue
){
    vector<int> matchingRows;
    for(int i = 0; i < table.rows.size(); i++){
        if(table.rows[i].values[columnIndex] == searchValue){
            matchingRows.push_back(i);
        }
    }
    return matchingRows;
}