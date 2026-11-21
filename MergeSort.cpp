#include "Database.h"
using namespace std;
void merge(
    vector<int>& indexes,
    vector<int>& temp,
    int left,
    int mid,
    int right,
    const Table& table,
    int columnIndex,
    bool ascending,
    bool numeric
){
    int i = left;
    int j = mid + 1;
    int k = left;
    while(i <= mid && j <= right){
        string first = table.rows[indexes[i]].values[columnIndex];
        string second = table.rows[indexes[j]].values[columnIndex];
        bool takeFirst;
        if(numeric){
            long long a = stoll(first);
            long long b = stoll(second);

            takeFirst = ascending ? a <= b : a >= b;
        }
        else{
            takeFirst = ascending
                ? first <= second
                : first >= second;
        }
        if(takeFirst){
            temp[k++] = indexes[i++];
        }
        else{
            temp[k++] = indexes[j++];
        }
    }
    while(i <= mid){
        temp[k++] = indexes[i++];
    }
    while(j <= right){
        temp[k++] = indexes[j++];
    }
    for(int x = left; x <= right; x++){
        indexes[x] = temp[x];
    }
}
void mergeSort(
    vector<int>& indexes,
    vector<int>& temp,
    int left,
    int right,
    const Table& table,
    int columnIndex,
    bool ascending,
    bool numeric
){
    if(left >= right){
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSort(
        indexes, temp, left, mid,
        table, columnIndex, ascending, numeric
    );
    mergeSort(
        indexes, temp, mid + 1, right,
        table, columnIndex, ascending, numeric
    );
    merge(
        indexes, temp, left, mid, right,
        table, columnIndex, ascending, numeric
    );
}
void sortRows(
    vector<int>& indexes,
    const Table& table,
    int columnIndex,
    bool ascending,
    bool numeric
){
    if(indexes.size() <= 1){
        return;
    }
    vector<int> temp(indexes.size());
    mergeSort(
        indexes,
        temp,
        0,
        static_cast<int>(indexes.size()) - 1,
        table,
        columnIndex,
        ascending,
        numeric
    );
}