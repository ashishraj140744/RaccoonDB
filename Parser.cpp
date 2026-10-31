#include "Parser.h"
#include <cctype>
using namespace std;
string toUpperCase(string word){
    for(int i = 0; i < word.length(); i++){
        word[i] = toupper(word[i]);
    }
    return word;
}
bool isComparisonOperator(string op){
    return op == "=" || op == "!=" || op == "<>" || op == ">" || op == "<" || op == ">=" || op == "<=";
}

bool parseSelect(
    const vector<string>& tokens, 
    vector<string>& selectedColumns, 
    string& tableName, 
    bool& hasWhere,
    string& conditionColumn,
    string& conditionOperator,
    string& conditionValue,
    string& errorMessage
){
    if(tokens.empty()){
        errorMessage = "Empty command.";
        return false;
    }
    if(toUpperCase(tokens[0]) != "SELECT"){
        errorMessage = "Command must start with SELECT.";
        return false;
    }
    selectedColumns.clear();
    tableName.clear();
    hasWhere = false;
    conditionColumn.clear();
    conditionOperator.clear();
    conditionValue.clear();
    int i = 1;
    if(i >= tokens.size() || tokens[i] == "FROM" || tokens[i] == "," || tokens[i] == ";"){
        errorMessage = "Expected a column name or * after SELECT.";
        return false;
    }
    if(tokens[i] == "*"){
        selectedColumns.push_back("*");
        i++;
        if(i >= tokens.size() || toUpperCase(tokens[i]) != "FROM"){
            errorMessage = "Expected FROM after *.";
            return false;
        }
    }
    else{
        while(i < tokens.size() && toUpperCase(tokens[i]) != "FROM"){
            if(tokens[i] == "," || tokens[i] == ";" || tokens[i] == "*"){
                errorMessage = "Expected a column name.";
                return false;
            }
            selectedColumns.push_back(tokens[i]);
            i++;
            if(i < tokens.size() && tokens[i] == ","){
                i++;
                if(i >= tokens.size() || tokens[i] == "," || tokens[i] == ";" || tokens[i] == "*" || toUpperCase(tokens[i]) == "FROM"){
                    errorMessage = "Expected a column name after comma.";
                    return false;
                }
            }
            else if(i < tokens.size() && toUpperCase(tokens[i]) != "FROM"){
                errorMessage = "Expected a comma or FROM after column name.";
                return false;
            }
        }
    }
    if(i >= tokens.size() || toUpperCase(tokens[i]) != "FROM"){
        errorMessage = "Expected FROM after the column list.";
        return false;
    }
    i++;
    if(i >= tokens.size() || tokens[i] == ";" || tokens[i] == "," || tokens[i] == "*"){
        errorMessage = "Expected a table name after FROM.";
        return false;
    }
    tableName = tokens[i];
    i++;
    if(i < tokens.size() && toUpperCase(tokens[i]) == "WHERE"){
        hasWhere = true;
        i++;
        if(i >= tokens.size() || tokens[i] == ";" || tokens[i] == "," || isComparisonOperator(tokens[i])){
            errorMessage = "Expected a column name after WHERE.";
            return false;
        }
        conditionColumn = tokens[i];
        i++;
        if(i >= tokens.size() || !isComparisonOperator(tokens[i])){
            errorMessage = "Expected a valid comparison operator after the WHERE column.";
            return false;
        }
        conditionOperator = tokens[i];
        i++;
        if(i >= tokens.size() || tokens[i] == ";" || tokens[i] == "," || isComparisonOperator(tokens[i])){
            errorMessage = "Expected a value after the WHERE operator.";
            return false;
        }
        conditionValue = tokens[i];
        i++;
    }
    if(i < tokens.size() && tokens[i] == ";"){
        i++;
    }
    if(i != tokens.size()){
        errorMessage = "Unexpected extra tokens after table name.";
        return false;
    }
    errorMessage = "SELECT command structure is valid.";
    return true;
}