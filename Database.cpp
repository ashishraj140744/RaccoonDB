#include "Database.h"
#include <cctype>
#include <limits>
void sortRows(
    vector<int>& indexes,
    const Table& table,
    int columnIndex,
    bool ascending,
    bool numeric
);
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
    row1.values = {"1", "Ashish", "19", "Shimla"};
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
string toUpper(string word);
bool isQuotedString(string value);
bool validateValue(string value, string columnType, string& errorMessage);
bool executeSelect(
    Table& table,
    vector<string> selectedColumns,
    const vector<Condition>& conditions,
    const vector<string>& logicalOperators,
    bool hasOrderBy,
    string orderByColumn,
    string orderDirection,
    string& errorMessage
){
    vector<int> columnIndexes;
    for(int i = 0; i < selectedColumns.size(); i++){
        bool found = false;
        for(int j = 0; j < table.columns.size(); j++){
            if(toUpper(table.columns[j].name) ==
                toUpper(selectedColumns[i])){
                columnIndexes.push_back(j);
                found = true;
                break;
            }
        }
        if(!found){
            errorMessage = "Column '" + selectedColumns[i] +
                           "' does not exist.";
            return false;
        }
    }
    if(logicalOperators.size() +
        (conditions.empty() ? 0 : 1) != conditions.size()){
        errorMessage = "Invalid WHERE condition structure.";
        return false;
    }
    vector<int> conditionIndexes;
    vector<string> compareValues;
    for(int i = 0; i < conditions.size(); i++){
        int conditionIndex = -1;
        for(int j = 0; j < table.columns.size(); j++){
            if(toUpper(table.columns[j].name) ==
                toUpper(conditions[i].column)){
                conditionIndex = j;
                break;
            }
        }
        if(conditionIndex == -1){
            errorMessage = "WHERE column '" +
                           conditions[i].column +
                           "' does not exist.";
            return false;
        }
        if(!validateValue(
            conditions[i].value,
            table.columns[conditionIndex].type,
            errorMessage
        )){
            errorMessage = "Invalid WHERE value for column '" +
                           table.columns[conditionIndex].name +
                           "': " + errorMessage;
            return false;
        }
        conditionIndexes.push_back(conditionIndex);
        string value = conditions[i].value;
        if(isQuotedString(value)){
            value = value.substr(1, value.length() - 2);
        }
        compareValues.push_back(value);
    }
    for(int i = 0; i < logicalOperators.size(); i++){
        if(logicalOperators[i] != "AND" &&
            logicalOperators[i] != "OR"){
            errorMessage = "Unsupported logical operator.";
            return false;
        }
    }
    int sortColumnIndex = -1;
    if(hasOrderBy){
        for(int i = 0; i < table.columns.size(); i++){
            if(toUpper(table.columns[i].name) ==
                toUpper(orderByColumn)){
                sortColumnIndex = i;
                break;
            }
        }
        if(sortColumnIndex == -1){
            errorMessage = "ORDER BY column '" +
                           orderByColumn +
                           "' does not exist.";
            return false;
        }
        orderDirection = toUpper(orderDirection);
        if(orderDirection != "ASC" &&
            orderDirection != "DESC"){
            errorMessage = "ORDER BY direction must be ASC or DESC.";
            return false;
        }
    }
    vector<int> matchingRows;
    for(int row = 0; row < table.rows.size(); row++){
        bool matches = true;
        if(!conditions.empty()){
            vector<bool> conditionResults;
            for(int c = 0; c < conditions.size(); c++){
                string rowValue =
                    table.rows[row].values[conditionIndexes[c]];
                string columnType =
                    toUpper(table.columns[conditionIndexes[c]].type);
                int comparison = 0;
                if(columnType == "INT"){
                    long long leftValue = stoll(rowValue);
                    long long rightValue = stoll(compareValues[c]);
                    if(leftValue < rightValue){
                        comparison = -1;
                    }
                    else if(leftValue > rightValue){
                        comparison = 1;
                    }
                }
                else{
                    if(rowValue < compareValues[c]){
                        comparison = -1;
                    }
                    else if(rowValue > compareValues[c]){
                        comparison = 1;
                    }
                }
                string op = conditions[c].comparisonOperator;
                bool result = false;
                if(op == "="){
                    result = (comparison == 0);
                }
                else if(op == "!=" || op == "<>"){
                    result = (comparison != 0);
                }
                else if(op == ">"){
                    result = (comparison > 0);
                }
                else if(op == "<"){
                    result = (comparison < 0);
                }
                else if(op == ">="){
                    result = (comparison >= 0);
                }
                else if(op == "<="){
                    result = (comparison <= 0);
                }
                else{
                    errorMessage = "Unsupported comparison operator.";
                    return false;
                }
                conditionResults.push_back(result);
            }
            bool currentAndGroup = conditionResults[0];
            bool anyOrGroupMatches = false;
            for(int c = 1; c < conditionResults.size(); c++){
                if(logicalOperators[c - 1] == "AND"){
                    currentAndGroup =
                        currentAndGroup && conditionResults[c];
                }
                else{
                    anyOrGroupMatches =
                        anyOrGroupMatches || currentAndGroup;
                    currentAndGroup = conditionResults[c];
                }
            }
            matches = anyOrGroupMatches || currentAndGroup;
        }
        if(matches){
            matchingRows.push_back(row);
        }
    }
    if(hasOrderBy){
        bool numeric =
            toUpper(table.columns[sortColumnIndex].type) == "INT";
        bool ascending = (orderDirection == "ASC");
        sortRows(
            matchingRows,
            table,
            sortColumnIndex,
            ascending,
            numeric
        );
    }
    for(int i = 0; i < columnIndexes.size(); i++){
        cout << table.columns[columnIndexes[i]].name;
        if(i != columnIndexes.size() - 1){
            cout << "\t";
        }
    }
    cout << endl;
    for(int rowIndex : matchingRows){
        for(int j = 0; j < columnIndexes.size(); j++){
            cout << table.rows[rowIndex].values[columnIndexes[j]];
            if(j != columnIndexes.size() - 1){
                cout << "\t";
            }
        }
        cout << endl;
    }
    if(!conditions.empty() && matchingRows.empty()){
        cout << "No matching records found." << endl;
    }
    errorMessage = "SELECT completed successfully.";
    return true;
}
bool createTable(
    Database& db,
    string tableName,
    const vector<string>& columnNames,
    const vector<string>& columnTypes,
    string& errorMessage
){
    if(findTable(db, tableName) != nullptr){
        errorMessage = "Table '" + tableName + "' already exists.";
        return false;
    }
    if(columnNames.empty() || columnNames.size() != columnTypes.size()){
        errorMessage = "Invalid table definition.";
        return false;
    }
    Table newTable;
    newTable.name = tableName;
    for(int i = 0; i < columnNames.size(); i++){
        Column col;
        col.name = columnNames[i];
        col.type = columnTypes[i];
        newTable.columns.push_back(col);
    }
    db.tables.push_back(newTable);
    errorMessage = "Table '" + tableName + "' created successfully.";
    return true;
}
string toUpper(string word){
    for(int i = 0; i < word.length(); i++){
        word[i] = toupper(static_cast<unsigned char>(word[i]));
    }
    return word;
}
bool isValidInteger(string value){
    if(value.empty()){
        return false;
    }
    int start = 0;
    if(value[0] == '-' || value[0] == '+'){
        start = 1;
    }
    if(start == value.length()){
        return false;
    }
    for(int i = start; i < value.length(); i++){
        if(!isdigit(static_cast<unsigned char>(value[i]))){
            return false;
        }
    }
    try{
        long long number = stoll(value);
        if(number < numeric_limits<int>::min() ||
           number > numeric_limits<int>::max()){
            return false;
        }
    }
    catch(...){
        return false;
    }
    return true;
}
bool isQuotedString(string value){
    if(value.length() < 2){
        return false;
    }
    char first = value[0];
    char last = value[value.length() - 1];
    return (first == '\'' && last == '\'') || (first == '"' && last == '"');
}
bool validateValue(string value, string columnType, string& errorMessage){
    string upperType = toUpper(columnType);
    if(upperType == "INT"){
        if(!isValidInteger(value)){
            errorMessage = "Expected an integer value, received '" + value + "'.";
            return false;
        }
        return true;
    }
    if(upperType == "TEXT"){
        if(!isQuotedString(value)){
            errorMessage = "TEXT values must be enclosed in quotes.";
            return false;
        }
        return true;
    }
    if(upperType.find("VARCHAR(") == 0){
        if(!isQuotedString(value)){
            errorMessage = "VARCHAR values must be enclosed in quotes.";
            return false;
        }
        size_t openBracket = upperType.find('(');
        size_t closeBracket = upperType.find(')');
        if(closeBracket == string::npos || closeBracket <= openBracket + 1){
            errorMessage = "Invalid VARCHAR definition.";
            return false;
        }
        int maxLength;
        try{
            maxLength = stoi(
                upperType.substr(
                    openBracket + 1,
                    closeBracket - openBracket - 1
                )
            );
        }
        catch(...){
            errorMessage = "Invalid VARCHAR length.";
            return false;
        }
        string actualValue = value.substr(1, value.length() - 2);
        if(actualValue.length() > maxLength){
            errorMessage = "Value exceeds VARCHAR(" +
                           to_string(maxLength) + ") limit.";
            return false;
        }
        return true;
    }
    errorMessage = "Unsupported column type: " + columnType;
    return false;
}
bool executeInsert(
    Database& db,
    string tableName,
    const vector<string>& columnNames,
    const vector<string>& values,
    string& errorMessage
){
    Table* table = findTable(db, tableName);
    if(table == nullptr){
        errorMessage = "Table '" + tableName + "' does not exist.";
        return false;
    }
    if(values.empty()){
        errorMessage = "INSERT requires values.";
        return false;
    }
    if(columnNames.size() != values.size() && !columnNames.empty()){
        errorMessage = "Number of columns and values does not match.";
        return false;
    }
    if(columnNames.empty() && values.size() != table->columns.size()){
        errorMessage = "Number of values must match the table's column count.";
        return false;
    }
    vector<string> newValues(table->columns.size());
    vector<bool> assigned(table->columns.size(), false);
    for(int i = 0; i < values.size(); i++){
        int columnIndex = -1;
        if(columnNames.empty()){
            columnIndex = i;
        }
        else{
            for(int j = 0; j < table->columns.size(); j++){
                if(toUpper(table->columns[j].name) ==
                   toUpper(columnNames[i])){
                    columnIndex = j;
                    break;
                }
            }
        }
        if(columnIndex == -1){
            errorMessage = "Column '" + columnNames[i] + "' does not exist.";
            return false;
        }
        if(assigned[columnIndex]){
            errorMessage = "Duplicate column: " +
                           table->columns[columnIndex].name;
            return false;
        }
        if(!validateValue(
            values[i],
            table->columns[columnIndex].type,
            errorMessage
        )){
            errorMessage = "Column '" + table->columns[columnIndex].name + "': " + errorMessage;
            return false;
        }
        string storedValue = values[i];
        if(isQuotedString(storedValue)){
            storedValue = storedValue.substr(
                1,
                storedValue.length() - 2
            );
        }
        newValues[columnIndex] = storedValue;
        assigned[columnIndex] = true;
    }
    for(int i = 0; i < assigned.size(); i++){
        if(!assigned[i]){
            errorMessage = "Missing value for column '" + table->columns[i].name + "'.";
            return false;
        }
    }
    Row newRow;
    newRow.values = newValues;
    table->rows.push_back(newRow);
    errorMessage = "1 row inserted successfully.";
    return true;
}
bool executeUpdate(
    Database& db,
    string tableName,
    const vector<Assignment>& assignments,
    const vector<Condition>& conditions,
    const vector<string>& logicalOperators,
    string& errorMessage
){
    Table* table = findTable(db, tableName);
    if(table == nullptr){
        errorMessage = "Table '" + tableName + "' does not exist.";
        return false;
    }
    if(assignments.empty()){
        errorMessage = "UPDATE requires at least one assignment.";
        return false;
    }
    if(logicalOperators.size() +
        (conditions.empty() ? 0 : 1) != conditions.size()){
        errorMessage = "Invalid WHERE condition structure.";
        return false;
    }
    vector<int> assignmentIndexes;
    vector<string> newValues;
    for(int i = 0; i < assignments.size(); i++){
        int columnIndex = -1;
        for(int j = 0; j < table->columns.size(); j++){
            if(toUpper(table->columns[j].name) ==
                toUpper(assignments[i].column)){
                columnIndex = j;
                break;
            }
        }
        if(columnIndex == -1){
            errorMessage = "Column '" +
                assignments[i].column +
                "' does not exist.";
            return false;
        }
        if(!validateValue(
            assignments[i].value,
            table->columns[columnIndex].type,
            errorMessage
        )){
            errorMessage = "Invalid value for column '" +
                table->columns[columnIndex].name +
                "': " + errorMessage;
            return false;
        }
        string value = assignments[i].value;
        if(isQuotedString(value)){
            value = value.substr(1, value.length() - 2);
        }
        assignmentIndexes.push_back(columnIndex);
        newValues.push_back(value);
    }
    vector<int> conditionIndexes;
    vector<string> compareValues;
    for(int i = 0; i < conditions.size(); i++){
        int columnIndex = -1;
        for(int j = 0; j < table->columns.size(); j++){
            if(toUpper(table->columns[j].name) ==
                toUpper(conditions[i].column)){
                columnIndex = j;
                break;
            }
        }
        if(columnIndex == -1){
            errorMessage = "WHERE column '" +
                conditions[i].column +
                "' does not exist.";
            return false;
        }
        if(!validateValue(
            conditions[i].value,
            table->columns[columnIndex].type,
            errorMessage
        )){
            errorMessage = "Invalid WHERE value for column '" +
                table->columns[columnIndex].name +
                "': " + errorMessage;
            return false;
        }
        conditionIndexes.push_back(columnIndex);
        string value = conditions[i].value;
        if(isQuotedString(value)){
            value = value.substr(1, value.length() - 2);
        }
        compareValues.push_back(value);
    }
    for(int i = 0; i < logicalOperators.size(); i++){
        if(logicalOperators[i] != "AND" &&
            logicalOperators[i] != "OR"){
            errorMessage = "Unsupported logical operator.";
            return false;
        }
    }
    vector<int> matchingRows;
    for(int row = 0; row < table->rows.size(); row++){
        bool matches = true;
        if(!conditions.empty()){
            vector<bool> conditionResults;
            for(int c = 0; c < conditions.size(); c++){
                string rowValue =
                    table->rows[row].values[conditionIndexes[c]];
                string columnType =
                    toUpper(table->columns[conditionIndexes[c]].type);
                int comparison = 0;
                if(columnType == "INT"){
                    long long leftValue = stoll(rowValue);
                    long long rightValue = stoll(compareValues[c]);
                    if(leftValue < rightValue){
                        comparison = -1;
                    }
                    else if(leftValue > rightValue){
                        comparison = 1;
                    }
                }
                else{
                    if(rowValue < compareValues[c]){
                        comparison = -1;
                    }
                    else if(rowValue > compareValues[c]){
                        comparison = 1;
                    }
                }
                string op = conditions[c].comparisonOperator;
                bool result = false;
                if(op == "="){
                    result = (comparison == 0);
                }
                else if(op == "!=" || op == "<>"){
                    result = (comparison != 0);
                }
                else if(op == ">"){
                    result = (comparison > 0);
                }
                else if(op == "<"){
                    result = (comparison < 0);
                }
                else if(op == ">="){
                    result = (comparison >= 0);
                }
                else if(op == "<="){
                    result = (comparison <= 0);
                }
                else{
                    errorMessage = "Unsupported comparison operator.";
                    return false;
                }
                conditionResults.push_back(result);
            }
            bool currentAndGroup = conditionResults[0];
            bool anyOrGroupMatches = false;
            for(int c = 1; c < conditionResults.size(); c++){
                if(logicalOperators[c - 1] == "AND"){
                    currentAndGroup =
                        currentAndGroup && conditionResults[c];
                }
                else{
                    anyOrGroupMatches =
                        anyOrGroupMatches || currentAndGroup;
                    currentAndGroup = conditionResults[c];
                }
            }
            matches = anyOrGroupMatches || currentAndGroup;
        }
        if(matches){
            matchingRows.push_back(row);
        }
    }
    for(int rowIndex : matchingRows){
        for(int i = 0; i < assignmentIndexes.size(); i++){
            table->rows[rowIndex].values[assignmentIndexes[i]] =
                newValues[i];
        }
    }
    errorMessage = to_string(matchingRows.size()) +
                   " row(s) updated successfully.";
    return true;
}
bool executeDelete(
    Database& db,
    string tableName,
    const vector<Condition>& conditions,
    const vector<string>& logicalOperators,
    string& errorMessage
){
    Table* table = findTable(db, tableName);
    if(table == nullptr){
        errorMessage = "Table '" + tableName + "' does not exist.";
        return false;
    }
    if(logicalOperators.size() +
        (conditions.empty() ? 0 : 1) != conditions.size()){
        errorMessage = "Invalid WHERE condition structure.";
        return false;
    }
    vector<int> conditionIndexes;
    vector<string> compareValues;
    for(int i = 0; i < conditions.size(); i++){
        int columnIndex = -1;
        for(int j = 0; j < table->columns.size(); j++){
            if(toUpper(table->columns[j].name) ==
                toUpper(conditions[i].column)){
                columnIndex = j;
                break;
            }
        }
        if(columnIndex == -1){
            errorMessage = "WHERE column '" +
                conditions[i].column +
                "' does not exist.";
            return false;
        }
        if(!validateValue(
            conditions[i].value,
            table->columns[columnIndex].type,
            errorMessage
        )){
            errorMessage = "Invalid WHERE value: " + errorMessage;
            return false;
        }
        conditionIndexes.push_back(columnIndex);
        string value = conditions[i].value;
        if(isQuotedString(value)){
            value = value.substr(1, value.length() - 2);
        }
        compareValues.push_back(value);
    }
    for(int i = 0; i < logicalOperators.size(); i++){
        if(logicalOperators[i] != "AND" &&
            logicalOperators[i] != "OR"){
            errorMessage = "Unsupported logical operator.";
            return false;
        }
    }
    vector<int> matchingRows;
    for(int row = 0; row < table->rows.size(); row++){
        bool matches = true;
        if(!conditions.empty()){
            vector<bool> conditionResults;
            for(int c = 0; c < conditions.size(); c++){
                string rowValue =
                    table->rows[row].values[conditionIndexes[c]];
                string columnType =
                    toUpper(table->columns[conditionIndexes[c]].type);
                int comparison = 0;
                if(columnType == "INT"){
                    long long leftValue = stoll(rowValue);
                    long long rightValue = stoll(compareValues[c]);
                    if(leftValue < rightValue){
                        comparison = -1;
                    }
                    else if(leftValue > rightValue){
                        comparison = 1;
                    }
                }
                else{
                    if(rowValue < compareValues[c]){
                        comparison = -1;
                    }
                    else if(rowValue > compareValues[c]){
                        comparison = 1;
                    }
                }
                string op = conditions[c].comparisonOperator;
                bool result = false;
                if(op == "="){
                    result = (comparison == 0);
                }
                else if(op == "!=" || op == "<>"){
                    result = (comparison != 0);
                }
                else if(op == ">"){
                    result = (comparison > 0);
                }
                else if(op == "<"){
                    result = (comparison < 0);
                }
                else if(op == ">="){
                    result = (comparison >= 0);
                }
                else if(op == "<="){
                    result = (comparison <= 0);
                }
                else{
                    errorMessage = "Unsupported comparison operator.";
                    return false;
                }
                conditionResults.push_back(result);
            }
            bool currentAndGroup = conditionResults[0];
            bool anyOrGroupMatches = false;
            for(int c = 1; c < conditionResults.size(); c++){
                if(logicalOperators[c - 1] == "AND"){
                    currentAndGroup =
                        currentAndGroup && conditionResults[c];
                }
                else{
                    anyOrGroupMatches =
                        anyOrGroupMatches || currentAndGroup;
                    currentAndGroup = conditionResults[c];
                }
            }
            matches = anyOrGroupMatches || currentAndGroup;
        }
        if(matches){
            matchingRows.push_back(row);
        }
    }
    for(int i = static_cast<int>(matchingRows.size()) - 1;
        i >= 0; i--){
        int rowIndex = matchingRows[i];
        table->rows.erase(table->rows.begin() + rowIndex);
    }
    errorMessage = to_string(matchingRows.size()) +
                   " row(s) deleted successfully.";
    return true;
}

bool createDatabase(
    DatabaseManager& manager,
    string databaseName,
    string& errorMessage
){
    for(int i = 0; i < manager.databases.size(); i++){
        if(toUpper(manager.databases[i].name) ==
            toUpper(databaseName)){
            errorMessage = "Database '" + databaseName +
                           "' already exists.";
            return false;
        }
    }
    Database newDatabase;
    newDatabase.name = databaseName;
    manager.databases.push_back(newDatabase);
    errorMessage = "Database '" + databaseName +
                   "' created successfully.";
    return true;
}
bool useDatabase(
    DatabaseManager& manager,
    string databaseName,
    string& errorMessage
){
    for(int i = 0; i < manager.databases.size(); i++){
        if(manager.databases[i].name == databaseName){
            manager.currentDatabaseIndex = i;
            errorMessage = "Switched to database '" + databaseName + "'.";
            return true;
        }
    }
    errorMessage = "Database '" + databaseName + "' does not exist.";
    return false;
}