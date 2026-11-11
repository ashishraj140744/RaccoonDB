#include "Database.h"
using namespace std;
class MergeSort{
public:
    static void sort(
        vector<int>& indexes,
        const Table& table,
        int columnIndex,
        bool ascending,
        bool numeric
    );
private:
    static void mergeSort(
        vector<int>& indexes,
        vector<int>& temp,
        int left,
        int right,
        const Table& table,
        int columnIndex,
        bool ascending,
        bool numeric
    );
    static void merge(
        vector<int>& indexes,
        vector<int>& temp,
        int left,
        int middle,
        int right,
        const Table& table,
        int columnIndex,
        bool ascending,
        bool numeric
    );
};
void MergeSort::sort(
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
void MergeSort::mergeSort(
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
    int middle = left + (right - left) / 2;
    mergeSort(
        indexes, temp, left, middle,
        table, columnIndex, ascending, numeric
    );
    mergeSort(
        indexes, temp, middle + 1, right,
        table, columnIndex, ascending, numeric
    );
    merge(
        indexes, temp, left, middle, right,
        table, columnIndex, ascending, numeric
    );
}
void MergeSort::merge(
    vector<int>& indexes,
    vector<int>& temp,
    int left,
    int middle,
    int right,
    const Table& table,
    int columnIndex,
    bool ascending,
    bool numeric
){
    int i = left;
    int j = middle + 1;
    int k = left;
    while(i <= middle && j <= right){
        string first =
            table.rows[indexes[i]].values[columnIndex];
        string second =
            table.rows[indexes[j]].values[columnIndex];
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
    while(i <= middle){
        temp[k++] = indexes[i++];
    }
    while(j <= right){
        temp[k++] = indexes[j++];
    }
    for(int x = left; x <= right; x++){
        indexes[x] = temp[x];
    }
}
void sortRows(
    vector<int>& indexes,
    const Table& table,
    int columnIndex,
    bool ascending,
    bool numeric
){
    MergeSort::sort(
        indexes,
        table,
        columnIndex,
        ascending,
        numeric
    );
}