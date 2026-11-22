#include "Database.h"
#include <map>
#include <memory>
using namespace std;
class BSTNode{
public:
    int key;
    vector<int> rowIndices;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int value, int rowIndex){
        key = value;
        rowIndices.push_back(rowIndex);
        left = nullptr;
        right = nullptr;
    }
};
class BinarySearchTree{
private:
    BSTNode* root;
    BSTNode* insertNode(BSTNode* node, int key, int rowIndex){
        if(node == nullptr){
            return new BSTNode(key, rowIndex);
        }
        if(key < node->key){
            node->left = insertNode(node->left, key, rowIndex);
        }
        else if(key > node->key){
            node->right = insertNode(node->right, key, rowIndex);
        }
        else{
            node->rowIndices.push_back(rowIndex);
        }
        return node;
    }
    void displayInOrder(BSTNode* node){
        if(node == nullptr){
            return;
        }
        displayInOrder(node->left);
        cout << "Value: " << node->key << " | Rows: ";
        for(int index : node->rowIndices){
            cout << index << " ";
        }
        cout << endl;
        displayInOrder(node->right);
    }
        void searchEqual(
        BSTNode* node,
        int key,
        vector<int>& results
    ){
        if(node == nullptr){
            return;
        }
        if(key < node->key){
            searchEqual(node->left, key, results);
        }
        else if(key > node->key){
            searchEqual(node->right, key, results);
        }
        else{
            for(int index : node->rowIndices){
                results.push_back(index);
            }
        }
    }
    void searchRange(
        BSTNode* node,
        int key,
        string comparisonOperator,
        vector<int>& results
    ){
        if(node == nullptr){
            return;
        }
        bool match = false;
        if(comparisonOperator == "!="){
            match = (node->key != key);
        }
        else if(comparisonOperator == ">"){
            match = (node->key > key);
        }
        else if(comparisonOperator == "<"){
            match = (node->key < key);
        }
        else if(comparisonOperator == ">="){
            match = (node->key >= key);
        }
        else if(comparisonOperator == "<="){
            match = (node->key <= key);
        }
        if(match){
            for(int index : node->rowIndices){
                results.push_back(index);
            }
        }
        if(comparisonOperator == "!="){
            searchRange(node->left, key, comparisonOperator, results);
            searchRange(node->right, key, comparisonOperator, results);
        }
        else if(comparisonOperator == ">"){
            searchRange(node->right, key, comparisonOperator, results);
            if(node->key > key){
                searchRange(node->left, key, comparisonOperator, results);
            }
        }
        else if(comparisonOperator == "<"){
            searchRange(node->left, key, comparisonOperator, results);
            if(node->key < key){
                searchRange(node->right, key, comparisonOperator, results);
            }
        }
        else if(comparisonOperator == ">="){
            searchRange(node->right, key, comparisonOperator, results);
            if(node->key >= key){
                searchRange(node->left, key, comparisonOperator, results);
            }
        }
        else if(comparisonOperator == "<="){
            searchRange(node->left, key, comparisonOperator, results);
            if(node->key <= key){
                searchRange(node->right, key, comparisonOperator, results);
            }
        }
    }
    void deleteTree(BSTNode* node){
        if(node == nullptr){
            return;
        }
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
public:
    BinarySearchTree(){
        root = nullptr;
    }
    ~BinarySearchTree(){
        deleteTree(root);
    }
    void insert(int key, int rowIndex){
        root = insertNode(root, key, rowIndex);
    }
    void display(){
        displayInOrder(root);
    }
    vector<int> search(int key){
        vector<int> results;
        searchEqual(root, key, results);
        return results;
    }
    vector<int> searchCondition(
        int key,
        string comparisonOperator
    ){
        vector<int> results;
        if(comparisonOperator == "="){
            searchEqual(root, key, results);
        }
        else{
            searchRange(
                root,
                key,
                comparisonOperator,
                results
            );
        }
        return results;
    }
};
map<string, unique_ptr<BinarySearchTree>> bstIndexes;
string makeBSTIndexKey(
    string databaseName,
    string tableName,
    int columnIndex
){
    return databaseName + "|" +
           tableName + "|" +
           to_string(columnIndex);
}
void buildBSTIndex(
    string databaseName,
    Table& table,
    int columnIndex
){
    if(columnIndex < 0 ||
       columnIndex >= table.columns.size()){
        return;
    }
    if(table.columns[columnIndex].type != "INT"){
        return;
    }
    string indexKey = makeBSTIndexKey(
        databaseName,
        table.name,
        columnIndex
    );
    unique_ptr<BinarySearchTree> bst =
        make_unique<BinarySearchTree>();
    for(int i = 0; i < table.rows.size(); i++){
        int value = stoi(
            table.rows[i].values[columnIndex]
        );
        bst->insert(value, i);
    }
    bstIndexes[indexKey] = move(bst);
}
bool searchBSTIndex(
    string databaseName,
    const Table& table,
    int columnIndex,
    int searchValue,
    string comparisonOperator,
    vector<int>& results
){
    string indexKey = makeBSTIndexKey(
        databaseName,
        table.name,
        columnIndex
    );
    auto it = bstIndexes.find(indexKey);
    if(it == bstIndexes.end()){
        return false;
    }
    results = it->second->searchCondition(
        searchValue,
        comparisonOperator
    );
    return true;
}