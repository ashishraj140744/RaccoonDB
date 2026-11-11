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
    vector<Condition>& conditions,
    vector<string>& logicalOperators,
    bool& hasOrderBy,
    string& orderByColumn,
    string& orderDirection,
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
    conditions.clear();
    logicalOperators.clear();
    hasOrderBy = false;
    orderByColumn = "";
    orderDirection = "ASC";
    int i = 1;    
    if(i >= tokens.size()){
        errorMessage = "Expected columns after SELECT.";
        return false;
    }
    if(tokens[i] == "*"){
        selectedColumns.push_back("*");
        i++;
    }
    else{
        while(i < tokens.size() &&
               toUpperCase(tokens[i]) != "FROM"){
            if(tokens[i] == "," || tokens[i] == ";" ||
                tokens[i] == "*"){
                errorMessage = "Expected a column name.";
                return false;
            }
            selectedColumns.push_back(tokens[i]);
            i++;
            if(i < tokens.size() && tokens[i] == ","){
                i++;
                if(i >= tokens.size() ||
                    toUpperCase(tokens[i]) == "FROM" ||
                    tokens[i] == "," || tokens[i] == ";"){
                    errorMessage = "Expected a column after comma.";
                    return false;
                }
            }
            else if(i < tokens.size() && toUpperCase(tokens[i]) != "FROM"){
                errorMessage = "Expected comma or FROM.";
                return false;
            }
        }
    }
    if(i >= tokens.size() ||
        toUpperCase(tokens[i]) != "FROM"){
        errorMessage = "Expected FROM.";
        return false;
    }
    i++;
    if(i >= tokens.size() || tokens[i] == ";" || tokens[i] == ","){
        errorMessage = "Expected table name.";
        return false;
    }
    tableName = tokens[i];
    i++;
    if(i < tokens.size() && toUpperCase(tokens[i]) == "WHERE"){
        i++;
        while (true){
            if(i >= tokens.size() || tokens[i] == ";" ||
                tokens[i] == "," ||
                isComparisonOperator(tokens[i]) ||
                toUpperCase(tokens[i]) == "AND" ||
                toUpperCase(tokens[i]) == "OR" ||
                toUpperCase(tokens[i]) == "ORDER"){
                errorMessage = "Expected a column in WHERE condition.";
                return false;
            }
            Condition condition;
            condition.column = tokens[i];
            i++;
            if(i >= tokens.size() ||
                !isComparisonOperator(tokens[i])){
                errorMessage = "Expected comparison operator.";
                return false;
            }
            condition.comparisonOperator = tokens[i];
            i++;
            if(i >= tokens.size() || tokens[i] == ";" || tokens[i] == "," ||
                isComparisonOperator(tokens[i]) ||
                toUpperCase(tokens[i]) == "AND" ||
                toUpperCase(tokens[i]) == "OR" ||
                toUpperCase(tokens[i]) == "ORDER"){
                errorMessage = "Expected comparison value.";
                return false;
            }
            condition.value = tokens[i];
            i++;
            conditions.push_back(condition);
            if(i < tokens.size() && (toUpperCase(tokens[i]) == "AND" || toUpperCase(tokens[i]) == "OR")){
                logicalOperators.push_back(
                    toUpperCase(tokens[i])
                );
                i++;
                continue;
            }
            break;
        }
    }
    if(i < tokens.size() && toUpperCase(tokens[i]) == "ORDER"){
        hasOrderBy = true;
        i++;
        if(i >= tokens.size() || toUpperCase(tokens[i]) != "BY"){
            errorMessage = "Expected BY after ORDER.";
            return false;
        }
        i++;
        if(i >= tokens.size() || tokens[i] == ";" || tokens[i] == ","){
            errorMessage = "Expected sorting column after ORDER BY.";
            return false;
        }
        orderByColumn = tokens[i];
        i++;
        if(i < tokens.size() && (toUpperCase(tokens[i]) == "ASC" || toUpperCase(tokens[i]) == "DESC")){
            orderDirection = toUpperCase(tokens[i]);
            i++;
        }
    }
    if(i < tokens.size() && tokens[i] == ";"){
        i++;
    }
    if(i != tokens.size()){
        errorMessage = "Unexpected extra tokens after SELECT statement.";
        return false;
    }
    errorMessage = "SELECT command structure is valid.";
    return true;
}
bool parseCreateTable(
    const vector<string>& tokens,
    string& tableName,
    vector<string>& columnNames,
    vector<string>& columnTypes,
    string& errorMessage
){
    tableName.clear();
    columnNames.clear();
    columnTypes.clear();
    if(tokens.empty()){
        errorMessage = "Empty command.";
        return false;
    }
    if(toUpperCase(tokens[0]) != "CREATE"){
        errorMessage = "Command must start with CREATE.";
        return false;
    }
    if(tokens.size() < 2 || toUpperCase(tokens[1]) != "TABLE"){
        errorMessage = "Expected TABLE after CREATE.";
        return false;
    }
    int i = 2;
    if(i >= tokens.size() || tokens[i] == "(" || tokens[i] == ")" || tokens[i] == "," || tokens[i] == ";"){
        errorMessage = "Expected a table name.";
        return false;
    }
    tableName = tokens[i];
    i++;
    if(i >= tokens.size() || tokens[i] != "("){
        errorMessage = "Expected '(' after table name.";
        return false;
    }
    i++;
    while(i < tokens.size()){
        if(tokens[i] == "," || tokens[i] == ")" || tokens[i] == ";"){
            errorMessage = "Expected a column name.";
            return false;
        }
        string columnName = tokens[i];
        i++;
        if(i >= tokens.size()){
            errorMessage = "Expected a column type.";
            return false;
        }
        string columnType = toUpperCase(tokens[i]);
        i++;
        if(columnType == "INT" || columnType == "TEXT"){
        }
        else if(columnType == "VARCHAR"){
            if(i >= tokens.size() || tokens[i] != "("){
                errorMessage = "Expected '(' after VARCHAR.";
                return false;
            }
            i++;
            if(i >= tokens.size() || tokens[i] == ")"){
                errorMessage = "Expected VARCHAR length.";
                return false;
            }
            string length = tokens[i];
            bool validLength = !length.empty();
            for(int j = 0; j < length.length(); j++){
                if(!isdigit(static_cast<unsigned char>(length[j]))){
                    validLength = false;
                    break;
                }
            }
            if(!validLength || length == "0"){
                errorMessage = "VARCHAR length must be a positive integer.";
                return false;
            }
            i++;
            if(i >= tokens.size() || tokens[i] != ")"){
                errorMessage = "Expected ')' after VARCHAR length.";
                return false;
            }
            i++;
            columnType = "VARCHAR(" + length + ")";
        }
        else{
            errorMessage = "Unsupported column type: " + columnType;
            return false;
        }
        for(int j = 0; j < columnNames.size(); j++){
            if(toUpperCase(columnNames[j]) == toUpperCase(columnName)){
                errorMessage = "Duplicate column name: " + columnName;
                return false;
            }
        }
        columnNames.push_back(columnName);
        columnTypes.push_back(columnType);
        if(i < tokens.size() && tokens[i] == ","){
            i++;
            if(i >= tokens.size() || tokens[i] == ")"){
                errorMessage = "Expected a column after comma.";
                return false;
            }
        }
        else if(i < tokens.size() && tokens[i] == ")"){
            break;
        }
        else{
            errorMessage = "Expected ',' or ')' after column definition.";
            return false;
        }
    }
    if(i >= tokens.size() || tokens[i] != ")"){
        errorMessage = "Expected ')' after column definitions.";
        return false;
    }
    i++;
    if(i < tokens.size() && tokens[i] == ";"){
        i++;
    }
    if(i != tokens.size()){
        errorMessage = "Unexpected extra tokens after CREATE TABLE.";
        return false;
    }
    errorMessage = "CREATE TABLE structure is valid.";
    return true;
}
bool parseInsert(
    const vector<string>& tokens,
    string& tableName,
    vector<string>& columnNames,
    vector<string>& values,
    string& errorMessage
){
    tableName.clear();
    columnNames.clear();
    values.clear();
    if(tokens.size() < 6 || toUpperCase(tokens[0]) != "INSERT"){
        errorMessage = "Invalid INSERT command.";
        return false;
    }
    if(toUpperCase(tokens[1]) != "INTO"){
        errorMessage = "Expected INTO after INSERT.";
        return false;
    }
    tableName = tokens[2];
    int i = 3;
    if(i < tokens.size() && tokens[i] == "("){
        i++;
        while(i < tokens.size() && tokens[i] != ")"){
            if(tokens[i] == "," || tokens[i] == ";"){
                errorMessage = "Invalid column name.";
                return false;
            }
            columnNames.push_back(tokens[i]);
            i++;
            if(i < tokens.size() && tokens[i] == ","){
                i++;
                if(i >= tokens.size() || tokens[i] == ")"){
                    errorMessage = "Expected column name after comma.";
                    return false;
                }
            }
            else if(i < tokens.size() && tokens[i] != ")"){
                errorMessage = "Expected ',' or ')' after column name.";
                return false;
            }
        }
        if(i >= tokens.size() || tokens[i] != ")"){
            errorMessage = "Expected ')' after column names.";
            return false;
        }
        i++;
    }
    if(i >= tokens.size() || toUpperCase(tokens[i]) != "VALUES"){
        errorMessage = "Expected VALUES.";
        return false;
    }
    i++;
    if(i >= tokens.size() || tokens[i] != "("){
        errorMessage = "Expected '(' after VALUES.";
        return false;
    }
    i++;
    while(i < tokens.size() && tokens[i] != ")"){
        if(tokens[i] == "," || tokens[i] == ";" || tokens[i] == "("){
            errorMessage = "Expected a value.";
            return false;
        }
        values.push_back(tokens[i]);
        i++;
        if(i < tokens.size() && tokens[i] == ","){
            i++;
            if(i >= tokens.size() || tokens[i] == ")"){
                errorMessage = "Expected value after comma.";
                return false;
            }
        }
        else if(i < tokens.size() && tokens[i] != ")"){
            errorMessage = "Expected ',' or ')' after value.";
            return false;
        }
    }
    if(values.empty()){
        errorMessage = "INSERT must contain at least one value.";
        return false;
    }
    if(i >= tokens.size() || tokens[i] != ")"){
        errorMessage = "Expected ')' after values.";
        return false;
    }
    i++;
    if(i < tokens.size() && tokens[i] == ";"){
        i++;
    }
    if(i != tokens.size()){
        errorMessage = "Unexpected tokens after INSERT.";
        return false;
    }
    if(!columnNames.empty() && columnNames.size() != values.size()){
        errorMessage = "Number of columns and values does not match.";
        return false;
    }
    errorMessage = "INSERT command structure is valid.";
    return true;
}

bool parseUpdate(
    const vector<string>& tokens,
    string& tableName,
    vector<Assignment>& assignments,
    vector<Condition>& conditions,
    vector<string>& logicalOperators,
    string& errorMessage
){
    tableName.clear();
    assignments.clear();
    conditions.clear();
    logicalOperators.clear();
    if(tokens.size() < 7 ||
        toUpperCase(tokens[0]) != "UPDATE"){
        errorMessage = "Invalid UPDATE command.";
        return false;
    }
    if(tokens[1] == "SET" || tokens[1] == ";" ||
        tokens[1] == ","){
        errorMessage = "Expected table name after UPDATE.";
        return false;
    }
    tableName = tokens[1];
    int i = 2;
    if(i >= tokens.size() ||
        toUpperCase(tokens[i]) != "SET"){
        errorMessage = "Expected SET after table name.";
        return false;
    }
    i++;
    while(i < tokens.size()){
        if(toUpperCase(tokens[i]) == "WHERE" ||
            tokens[i] == ";" ||
            tokens[i] == ","){
            errorMessage = "Expected column name in SET.";
            return false;
        }
        Assignment assignment;
        assignment.column = tokens[i];
        i++;
        if(i >= tokens.size() || tokens[i] != "="){
            errorMessage = "Expected '=' after column name.";
            return false;
        }
        i++;
        if(i >= tokens.size() ||
            tokens[i] == "," ||
            tokens[i] == ";" ||
            toUpperCase(tokens[i]) == "WHERE"){
            errorMessage = "Expected value after '='.";
            return false;
        }
        assignment.value = tokens[i];
        i++;
        for(int j = 0; j < assignments.size(); j++){
            if(toUpperCase(assignments[j].column) ==
                toUpperCase(assignment.column)){
                errorMessage = "Column '" +
                    assignment.column +
                    "' is assigned more than once.";
                return false;
            }
        }
        assignments.push_back(assignment);
        if(i < tokens.size() && tokens[i] == ","){
            i++;
            if(i >= tokens.size() ||
                tokens[i] == ";" ||
                toUpperCase(tokens[i]) == "WHERE"){

                errorMessage = "Expected another assignment after comma.";
                return false;
            }
        }
        else{
            break;
        }
    }
    if(assignments.empty()){
        errorMessage = "UPDATE must contain at least one assignment.";
        return false;
    }
    if(i < tokens.size() &&
        toUpperCase(tokens[i]) == "WHERE"){
        i++;
        while(true){
            if(i >= tokens.size() ||
                tokens[i] == ";" ||
                tokens[i] == "," ||
                isComparisonOperator(tokens[i]) ||
                toUpperCase(tokens[i]) == "AND" ||
                toUpperCase(tokens[i]) == "OR"){
                errorMessage = "Expected column in WHERE condition.";
                return false;
            }
            Condition condition;
            condition.column = tokens[i];
            i++;
            if(i >= tokens.size() ||
                !isComparisonOperator(tokens[i])){
                errorMessage = "Expected comparison operator in WHERE.";
                return false;
            }
            condition.comparisonOperator = tokens[i];
            i++;
            if(i >= tokens.size() ||
                tokens[i] == ";" ||
                tokens[i] == "," ||
                isComparisonOperator(tokens[i]) ||
                toUpperCase(tokens[i]) == "AND" ||
                toUpperCase(tokens[i]) == "OR"){
                errorMessage = "Expected value in WHERE condition.";
                return false;
            }
            condition.value = tokens[i];
            i++;
            conditions.push_back(condition);
            if(i < tokens.size() &&
                (toUpperCase(tokens[i]) == "AND" ||
                 toUpperCase(tokens[i]) == "OR")){

                logicalOperators.push_back(
                    toUpperCase(tokens[i])
                );
                i++;
                continue;
            }
            break;
        }
    }
    if(i < tokens.size() && tokens[i] == ";"){
        i++;
    }
    if(i != tokens.size()){
        errorMessage = "Unexpected extra tokens after UPDATE statement.";
        return false;
    }
    errorMessage = "UPDATE command structure is valid.";
    return true;
}
bool parseDelete(
    const vector<string>& tokens,
    string& tableName,
    vector<Condition>& conditions,
    vector<string>& logicalOperators,
    string& errorMessage
){
    tableName.clear();
    conditions.clear();
    logicalOperators.clear();
    if(tokens.size() < 3 ||
        toUpperCase(tokens[0]) != "DELETE"){
        errorMessage = "Invalid DELETE command.";
        return false;
    }
    if(toUpperCase(tokens[1]) != "FROM"){
        errorMessage = "Expected FROM after DELETE.";
        return false;
    }
    if(tokens[2] == ";" ||
        tokens[2] == "," ||
        toUpperCase(tokens[2]) == "WHERE"){
        errorMessage = "Expected table name after FROM.";
        return false;
    }
    tableName = tokens[2];
    int i = 3;
    if(i < tokens.size() &&
        toUpperCase(tokens[i]) == "WHERE"){
        i++;
        while(true){
            if(i >= tokens.size() ||
                tokens[i] == ";" ||
                tokens[i] == "," ||
                isComparisonOperator(tokens[i]) ||
                toUpperCase(tokens[i]) == "AND" ||
                toUpperCase(tokens[i]) == "OR"){
                errorMessage = "Expected column in WHERE condition.";
                return false;
            }
            Condition condition;
            condition.column = tokens[i];
            i++;
            if(i >= tokens.size() ||
                !isComparisonOperator(tokens[i])){
                errorMessage = "Expected comparison operator in WHERE.";
                return false;
            }
            condition.comparisonOperator = tokens[i];
            i++;
            if(i >= tokens.size() ||
                tokens[i] == ";" ||
                tokens[i] == "," ||
                isComparisonOperator(tokens[i]) ||
                toUpperCase(tokens[i]) == "AND" ||
                toUpperCase(tokens[i]) == "OR"){
                errorMessage = "Expected value in WHERE condition.";
                return false;
            }
            condition.value = tokens[i];
            i++;
            conditions.push_back(condition);
            if(i < tokens.size() &&
                (toUpperCase(tokens[i]) == "AND" ||
                 toUpperCase(tokens[i]) == "OR")){
                logicalOperators.push_back(
                    toUpperCase(tokens[i])
                );
                i++;
                continue;
            }
            break;
        }
    }
    if(i < tokens.size() && tokens[i] == ";"){
        i++;
    }
    if(i != tokens.size()){
        errorMessage =
            "Unexpected extra tokens after DELETE statement.";
        return false;
    }
    errorMessage = "DELETE command structure is valid.";
    return true;
}