#include "Parser.h"
#include <cctype>
using namespace std;
string toUpperCase(string word){
    for(int i = 0; i < word.length(); i++){
        word[i] = toupper(word[i]);
    }
    return word;
}
bool parseSelect(const vector<string>& tokens, vector<string>& selectedColumns, string& tableName, string& errorMessage){
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