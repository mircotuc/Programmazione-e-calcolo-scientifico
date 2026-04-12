#pragma once
#include <vector>
#include <string>
#include <iostream>

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