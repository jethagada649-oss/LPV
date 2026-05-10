// to compile use : g++ -fopenmp bubblesort.cpp
// to run use : ./a.out
#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>

using namespace std;

// --------------------------------------------------
// Sequential Bubble Sort
// --------------------------------------------------

void sequentialBubbleSort(vector<int>& arr) {

    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {

        for (int j = 0; j < n - i - 1; j++) {

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// --------------------------------------------------
// Parallel Bubble Sort using OpenMP
// Odd-Even Transposition Sort
// --------------------------------------------------

void parallelBubbleSort(vector<int>& arr) {

    int n = arr.size();

    for (int phase = 0; phase < n; phase++) {

        // Even phase
        if (phase % 2 == 0) {

            #pragma omp parallel for
            for (int i = 0; i < n - 1; i += 2) {

                if (arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                }
            }
        }

        // Odd phase
        else {

            #pragma omp parallel for
            for (int i = 1; i < n - 1; i += 2) {

                if (arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                }
            }
        }
    }
}

// --------------------------------------------------
// Utility Function
// --------------------------------------------------

void printArray(vector<int>& arr) {

    for (int num : arr) {
        cout << num << " ";
    }

    cout << endl;
}

// --------------------------------------------------
// Main Function
// --------------------------------------------------

int main() {

    int n = 10;

    vector<int> arr1(n), arr2(n);

    // Generate random numbers
    for (int i = 0; i < n; i++) {

        int value = rand() % 10000;

        arr1[i] = value;
        arr2[i] = value;
    }

    cout<<"Array is : ";
    for(int i = 0 ; i < n ; i++)
      cout<<arr1[i]<<" ";
    cout<<"\n";
    // Sequential Bubble Sort Timing
    double start1 = omp_get_wtime();

    sequentialBubbleSort(arr1);

    double end1 = omp_get_wtime();

    // Parallel Bubble Sort Timing
    double start2 = omp_get_wtime();

    parallelBubbleSort(arr2);

    double end2 = omp_get_wtime();

    // --------------------------------------------------

    cout << "Sequential Bubble Sort Time: "
         << end1 - start1
         << " seconds\n";

    cout << "Parallel Bubble Sort Time: "
         << end2 - start2
         << " seconds\n";
    cout<<"sorted Array is : ";
    for(int i = 0 ; i < n ; i++)
      cout<<arr2[i]<<" ";
    cout<<"\n";

    return 0;
}
