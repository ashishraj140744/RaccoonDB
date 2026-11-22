#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Node{
public:
    string key;
    vector<int> rowIndices;
    Node* next;
    Node(string k, int rowIndex){
        key = k;
        rowIndices.push_back(rowIndex);
        next = nullptr;
    }
};
class HashTable{
private:
    static const int SIZE = 10;
    Node* table[SIZE];
    int hashFunction(string key){
        int hashValue = 0;
        for(char ch : key){
            hashValue = (hashValue * 31 + static_cast<unsigned char>(ch)) % SIZE;
        }
        return hashValue;
    }
public:
    HashTable(){
        for(int i = 0; i < SIZE; i++){
            table[i] = nullptr;
        }
    }
    void insert(string key, int rowIndex){
        int index = hashFunction(key);
        Node* temp = table[index];
        while(temp != nullptr){
            if(temp->key == key){
                for(int existingIndex : temp->rowIndices){
                    if(existingIndex == rowIndex){
                        return;
                    }
                }
                temp->rowIndices.push_back(rowIndex);
                return;
            }
            temp = temp->next;
        }
        Node* newNode = new Node(key, rowIndex);
        newNode->next = table[index];
        table[index] = newNode;
    }
    bool search(string key, vector<int>& result){
        int index = hashFunction(key);
        Node* temp = table[index];
        while(temp != nullptr){
            if (temp->key == key) {
                result = temp->rowIndices;
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
    void remove(string key, int rowIndex){
        int index = hashFunction(key);
        Node* temp = table[index];
        Node* prev = nullptr;
        while(temp != nullptr){
            if(temp->key == key){
                for(int i = 0; i < (int)temp->rowIndices.size(); i++){
                    if(temp->rowIndices[i] == rowIndex){
                        temp->rowIndices.erase(
                            temp->rowIndices.begin() + i
                        );
                        if(temp->rowIndices.empty()){
                            if (prev == nullptr) {
                                table[index] = temp->next;
                            }
                            else{
                                prev->next = temp->next;
                            }
                            delete temp;
                        }
                        return;
                    }
                }
                return;
            }
            prev = temp;
            temp = temp->next;
        }
    }
    void display(){
        cout << "\nHash Table:\n";
        for(int i = 0; i < SIZE; i++){
            cout << i << " : ";
            Node* temp = table[i];
            while(temp != nullptr){
                cout << "(" << temp->key << " -> ";
                for(int rowIndex : temp->rowIndices){
                    cout << rowIndex << " ";
                }
                cout << ")";
                if(temp->next != nullptr){
                    cout << " -> ";
                }
                temp = temp->next;
            }
            cout << endl;
        }
    }
    void clear(){
        for(int i = 0; i < SIZE; i++){
            Node* temp = table[i];
            while (temp != nullptr){
                Node* nextNode = temp->next;
                delete temp;
                temp = nextNode;
            }
            table[i] = nullptr;
        }
    }
    ~HashTable(){
        clear();
    }
};