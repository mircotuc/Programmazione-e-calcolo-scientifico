#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <cmath> //per floor
#include <limits> //per inf

template <typename T>
void print_vector(const std::vector<T>& vec) {
    std::cout << "[ ";
    for (const T& element : vec) {
        std::cout << element << " ";
    }
    std::cout << "]" << std::endl;
}



// function to verify the vector is sorted
template <typename T>
bool is_sorted(const std::vector<T>& vec) {
    if (vec.size()<2) {
        return true; // A vector with 0 or 1 element is always sorted
    }
    for (size_t i = 1; i < vec.size(); i++) {
        if (vec[i] < vec[i - 1]) {
            return false;
        }
    }
    return true;
}


// Bubbble Sort
template <typename T>
void BubbleSort(std::vector<T>& vec) { // don't put const because otherwise
    // it cannot modify it
    if (vec.size() < 2) {
        return; // in order to avoid n-1 being negative
    }
    size_t n = vec.size();
    for (size_t i = 0; i < n-1; i++) {
        for (size_t j = n-1; j > i; j--) {
            if (vec[j] < vec[j-1]) {
                // Swap vec[j] and vec[j-1]
                T temporary = vec[j];
                vec[j] = vec[j-1];
                vec[j-1] = temporary;
            }
        }
    }
}

// Insertion Sort
template <typename T>
void InsertionSort(std::vector<T>& vec) {
    if (vec.size() < 2) {
        return; 
    }
    size_t n = vec.size();
    for (size_t j = 1; j < n; j++) {
        T key = vec[j];
        size_t i = j ;
        while (i > 0 && vec[i-1] > key) {
            vec[i] = vec[i-1];
            i=i-1;
        }
        vec[i] = key;
    }
}

// Selection Sort
template <typename T>
void SelectionSort(std::vector<T>& vec) {
        if (vec.size() < 2) {
        return; // in order to avoid n-1 being negative
    }
    size_t n = vec.size();
    for (size_t i = 0; i < n-1; i++) {
        size_t min_index = i;
        for (size_t j = i+1; j < n; j++) {
            if (vec[j] < vec[min_index]) {
                min_index = j;
            }
        }
        // Swap vec[i] and vec[min_index]
        T temporary = vec[i];
        vec[i] = vec[min_index];
        vec[min_index] = temporary;
    }
}
void Infinito(std::vector<std::string>& vec, size_t i){
    vec[i]= std::string(1000, '\xFF');
}
void Infinito(std::vector<int>& vec, size_t i){
    vec[i]= std::numeric_limits<int>::max();
}
//Merge
template <typename T>
void Merge(std::vector<T>& vec, size_t p, size_t q, size_t r) {
    size_t n1= q-p+1;
    size_t n2= r-q;
    std::vector<T> L(n1+1); // create arrays as vectors with dimensions n1+1 and n2+1
    std::vector<T> R(n2+1);
    for (size_t i=0; i<n1; i++) {
        L[i]=vec[p+i]; // lo pseudo codice è sfasato di 1
        // inizializzo solo i primi n1 elementi, l'ultimo lo metto poi uguale a inf
    }
    for (size_t j=0; j<n2; j++) {
        R[j]=vec[q+j+1];
    }
    Infinito(L,n1); //I used two omonymous functions, c++ automatically
    //chooses the right one
    Infinito(R,n2);
    size_t i=0;
    size_t j=0;
    for (size_t k=p; k<r+1;k++) {
        if (L[i]<=R[j]) {
            vec[k]=L[i];
            i=i+1;
        }
        else {
            vec[k]=R[j];
            j=j+1;
        }
    }
}

// Merge Sort
template <typename T>
void MergeSort(std::vector<T>& vec, size_t p, size_t r) {
    if (vec.size() < 2) {
        return; 
    }
    if (p<r) {
        size_t q= std::floor((p+r)/2);
        MergeSort(vec,p,q);
        MergeSort(vec, q+1, r);
        Merge(vec,p,q,r);
    }
}

// Partition
template <typename T>
size_t Partition(std::vector<T>& vec, size_t p, size_t r) { //it returns i
    T x= vec[r];
    size_t i=p; //not p-1 to avoid seg fault
    for (size_t j=p; j<r; j++) {
        if (vec[j]<=x) {
            T temp= vec[i]; //swap vec[i] e vec[j]
            vec[i]= vec[j];
            vec[j]= temp;
            i=i+1;
        }
    }
    T temporary= vec[i]; //swap vec[i] and vec[r]
    vec[i]= vec[r];
    vec[r]= temporary;
    return i; //since I changed the positio of i=i+1
}

// Quick Sort
template <typename T>
void QuickSort(std::vector<T>& vec, size_t p, size_t r) {
    if (vec.size() < 2) {
        return; 
    }
    if (p<r) {
        size_t q= Partition(vec,p,r);
        if (q>p) {//to avoid segfault
        QuickSort(vec,p,q-1);
        }
        if (q<r) { //not strictly necessary but it avoids useless computing if q=r
        QuickSort(vec,q+1, r);
        }
    }
}
// i need this function because otherwise the insertion sort would be called recorsively and work on every
// block that quicksort creates
template <typename T>
void QuickSortIntermediate(std::vector<T>& vec, size_t p, size_t r) {
    if (p>=r) return; // otherwise I risk r-p+1 being negative
    if (r-p+1<32) return; // in this way it goes directly to insertionsort
    // no need to put if p<r because we put the condition above
    size_t q= Partition(vec,p,r); 
    if (q>p) {//to avoid segfault
    QuickSortIntermediate(vec,p,q-1);
    }
    if (q<r) {
    QuickSortIntermediate(vec,q+1, r);
    }
}
// Quick Sort Optimus Prime
template <typename T>
void QuickSortOptimusPrime(std::vector<T>& vec) {
    if (vec.size() < 2) {
        return; 
    }
    QuickSortIntermediate(vec, 0, vec.size()-1);
    InsertionSort(vec); // it works on a semi-ordered vector so it is way faster
}

