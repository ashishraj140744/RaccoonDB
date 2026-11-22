#include "Database.h"
int main(){
    HashTable ht;
    ht.insert("Rahul", 0);
    ht.insert("Aman", 1);
    ht.insert("Rahul", 2);
    ht.insert("Priya", 3);
    ht.display();
    vector<int> result;
    cout << "\nSearching Rahul:\n";
    if(ht.search("Rahul", result)){
        for(int index : result){
            cout << index << " ";
        }
    }
    else{
        cout << "Not found";
    }
    cout << "\n\nRemoving Rahul at row 0:\n";
    ht.remove("Rahul", 0);
    ht.display();
    return 0;
}