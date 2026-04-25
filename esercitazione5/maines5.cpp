#include "sorting_algoritms.h"
#include "timecounter.h"
#include "randfiller.h"
#include <algorithm>
using namespace std;
int main() { // int in fornt of main stands for the 0 or the 1 of return
    timecounter chrono;
    randfiller rf;
for (size_t i=2; i<101; i++){
    vector<vector<int>> matrice_vettori(100, vector<int>(i,0)); //vector of vectors

    for (size_t j=0; j<100;j++){
        rf.fill(matrice_vettori[j],-10000, 10000);
    }

    vector<vector<int>> vecBubble(matrice_vettori);
    vector<vector<int>> vecInsertion(matrice_vettori);
    vector<vector<int>> vecSelection(matrice_vettori);
    vector<vector<int>> vecMerge(matrice_vettori);
    vector<vector<int>> vecQuick(matrice_vettori);
    vector<vector<int>> vecQuickOptimus(matrice_vettori);

    chrono.tic();
    for (size_t j=0; j<100;j++) {
        BubbleSort(vecBubble[j]);
    }
    double elapsed_BubbleSort = chrono.toc()/100;

    chrono.tic();
    for (size_t j=0; j<100;j++) {
        InsertionSort(vecInsertion[j]);
    }
    double elapsed_InsertionSort = chrono.toc()/100;
    
    chrono.tic();
    for (size_t j=0; j<100;j++) {
        SelectionSort(vecSelection[j]);
    }
    double elapsed_SelectionSort = chrono.toc()/100;
    
    chrono.tic();
    for (size_t j=0; j<100;j++) {
        MergeSort(vecMerge[j],0,vecMerge[j].size()-1);
    }
    double elapsed_MergeSort = chrono.toc()/100;

    chrono.tic();
    for (size_t j=0; j<100;j++) {
        QuickSort(vecQuick[j],0,vecQuick[j].size()-1);
    }
    double elapsed_QuickSort = chrono.toc()/100;
    
    chrono.tic();
    for (size_t j=0; j<100;j++) {
        QuickSortOptimusPrime(vecQuickOptimus[j]);
    }
    double elapsed_QuickSortOptimusPrime = chrono.toc()/100;
    
    cout<< "Dimensione:" << i << endl;
    cout<< "BubbleSort: " << elapsed_BubbleSort << " seconds" << endl;
    cout<< "InsertionSort: " << elapsed_InsertionSort << " seconds" << endl;
    cout<< "SelectionSort: " << elapsed_SelectionSort << " seconds" << endl;
    cout<< "MergeSort: " << elapsed_MergeSort << " seconds" << endl;
    cout<< "QuickSort: " << elapsed_QuickSort << " seconds" << endl;
    cout<< "QuickSortOptimusPrime: " << elapsed_QuickSortOptimusPrime << " seconds" << endl;
    chrono.tic();
    for (size_t j=0; j<100;j++) {
        sort(matrice_vettori[j].begin(), matrice_vettori[j].end());
    }
    double elapsed_Sort = chrono.toc()/100;
    cout<< "sort: " << elapsed_Sort << " seconds" << endl;
    double minimo= std::min({elapsed_BubbleSort, elapsed_InsertionSort, elapsed_SelectionSort, elapsed_MergeSort, elapsed_QuickSort, elapsed_Sort, elapsed_QuickSortOptimusPrime});
    if (minimo==elapsed_BubbleSort) {
        cout<< "Vince BubbleSort"<<endl;
    }
    if (minimo==elapsed_InsertionSort) {
        cout<< "Vince InsertionSort"<<endl;
    }
    if (minimo==elapsed_SelectionSort) {
        cout<< "Vince SelectionSort"<<endl;
    }
    if (minimo==elapsed_MergeSort) {
        cout<< "Vince MergeSort"<<endl;
    }
    if (minimo==elapsed_QuickSort) {
        cout<< "Vince QuickSort"<<endl;
    }
    if (minimo==elapsed_QuickSortOptimusPrime) {
        cout<< "Vince QuickSortOptimusPrime"<<endl;
    }
    if (minimo==elapsed_Sort) {
        cout<< "Vince Sort"<<endl;
    }
    }
return 0;
}