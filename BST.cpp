#include <iostream>
#include <string>
#include <vector>
using namespace std;
class BSTNode {
public:
    int key;
    vector<int> rowIndices;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int value, int rowIndex) {
        key = value;
        rowIndices.push_back(rowIndex);
        left = nullptr;
        right = nullptr;
    }
};
class BinarySearchTree {
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
};
int main() {
    BinarySearchTree bst;
    bst.insert(22, 0);
    bst.insert(18, 1);
    bst.insert(25, 2);
    bst.insert(19, 3);
    bst.insert(22, 4);
    bst.insert(21, 5);
    cout << "BST Inorder Traversal:\n";
    bst.display();
    return 0;
}