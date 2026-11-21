#ifndef STORAGE_H
#define STORAGE_H
#include "Database.h"
#include <string>
using namespace std;
bool saveDatabaseManager(
    const DatabaseManager& manager,
    const string& folderPath,
    string& errorMessage
);
bool loadDatabaseManager(
    DatabaseManager& manager,
    const string& folderPath,
    string& errorMessage
);
#endif