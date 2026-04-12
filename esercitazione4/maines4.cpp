#include "sorting_algoritms.h"
#include "timecounter.h"
#include "randfiller.h"
#include <algorithm>
using namespace std;
int main() { // int in fornt of main stands for the 0 or the 1 of return
    timecounter chrono;
    
for (size_t i=4; i<8192; i=i*2){
    
    vector<int> vec(i);
    randfiller rf;
    rf.fill(vec,-10000, 10000);
    vector<int> vecBubble(vec);
    vector<int> vecInsertion(vec);
    vector<int> vecSelection(vec);
    chrono.tic();
    BubbleSort(vecBubble);
    double elapsed_BubbleSort = chrono.toc();
    chrono.tic();
    InsertionSort(vecInsertion);
    double elapsed_InsertionSort = chrono.toc();
    chrono.tic();
    SelectionSort(vecSelection);    
    double elapsed_SelectionSort = chrono.toc();
    cout<< "BubbleSort: " << elapsed_BubbleSort << " seconds" << endl;
    cout<< "InsertionSort: " << elapsed_InsertionSort << " seconds" << endl;
    cout<< "SelectionSort: " << elapsed_SelectionSort << " seconds" << endl;
    chrono.tic();
    sort(vec.begin(), vec.end());
    double elapsed_Sort = chrono.toc();
    cout<< "sort: " << elapsed_Sort << " seconds" << endl;
}
return 0;
}