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
vector<int> linearSearchCondition(
    const Table& table,
    int columnIndex,
    string searchValue,
    string comparisonOperator,
    bool numeric
){
    vector<int> matchingRows;
    for(int i = 0; i < table.rows.size(); i++){
        string rowValue = table.rows[i].values[columnIndex];
        int comparison = 0;
        if(numeric){
            long long leftValue = stoll(rowValue);
            long long rightValue = stoll(searchValue);
            if(leftValue < rightValue){
                comparison = -1;
            }
            else if(leftValue > rightValue){
                comparison = 1;
            }
        }
        else{
            if(rowValue < searchValue){
                comparison = -1;
            }
            else if(rowValue > searchValue){
                comparison = 1;
            }
        }
        bool matches = false;
        if(comparisonOperator == "="){
            matches = (comparison == 0);
        }
        else if(comparisonOperator == "!=" ||
                comparisonOperator == "<>"){
            matches = (comparison != 0);
        }
        else if(comparisonOperator == ">"){
            matches = (comparison > 0);
        }
        else if(comparisonOperator == "<"){
            matches = (comparison < 0);
        }
        else if(comparisonOperator == ">="){
            matches = (comparison >= 0);
        }
        else if(comparisonOperator == "<="){
            matches = (comparison <= 0);
        }
        if(matches){
            matchingRows.push_back(i);
        }
    }
    return matchingRows;
}