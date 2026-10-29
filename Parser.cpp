#include "Parser.h"
#include <cctype>
using namespace std;
string toUpperCase(string word){
    for(int i = 0; i < word.length(); i++){
        word[i] = toupper(word[i]);
    }
    return word;
}
bool parseSelect(const vector<string>& tokens, string& selectedColumn, string& tableName, string& errorMessage){
    if(tokens.empty()){
        errorMessage = "Empty command.";
        return false;
    }
    if(toUpperCase(tokens[0]) != "SELECT"){
        errorMessage = "Command must start with SELECT.";
        return false;
    }
    if(tokens.size() < 4){
        errorMessage = "Incomplete SELECT command.";
        return false;
    }
    if(tokens[1] == "FROM" || tokens[1] == ";" || tokens[1] == ",") {
        errorMessage = "Expected a column name or * after SELECT.";
        return false;
    }
    if(toUpperCase(tokens[2]) != "FROM"){
        errorMessage = "Expected FROM after the column name.";
        return false;
    }
    if(tokens[3] == ";" || tokens[3] == "," || tokens[3] == "*"){
        errorMessage = "Expected a table name after FROM.";
        return false;
    }
    if(tokens.size() == 5 && tokens[4] != ";"){
        errorMessage = "Unexpected token after the table name.";
        return false;
    }
    if(tokens.size() > 5){
        errorMessage = "Unexpected extra tokens in SELECT command.";
        return false;
    }
    selectedColumn = tokens[1];
    tableName = tokens[3];
    errorMessage = "SELECT command structure is valid.";
    return true;
}