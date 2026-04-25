#include "sorting_algoritms.h"
#include "randfiller.h"
#include <iostream>
using namespace std;

int main() {
randfiller rf;
vector<int> dim(100);
rf.fill(dim,0,5000);
for (size_t i=0; i<100; i++) {
    vector<int> vec(dim[i]);
    rf.fill(vec,-10000, 10000);
    MergeSort(vec,0,vec.size()-1);
    //print_vector(vec);

    bool flag = is_sorted(vec);
    if (!flag) {
        return EXIT_FAILURE;
    }

}

vector<string> str= {"Torino", "Collegno", "Altamura", "Auckland",
     "Boston", "Francavilla", "Caserta","San Severo", "Wellington", "Santena" };
MergeSort(str,0,str.size()-1);

print_vector(str);
bool flag = is_sorted(str);
if (!flag) {
    return EXIT_FAILURE;
}
cout<<"MergeSort test passed successfully!"<<endl;
return EXIT_SUCCESS;

}