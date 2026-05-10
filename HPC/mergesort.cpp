// to compile use : g++ -fopenmp mergesort.cpp
// to run use : ./a.out
#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>

using namespace std;

// --------------------------------------------------
// Merge Function
// --------------------------------------------------

void merge(vector<int>& arr, int left, int mid, int right) {

    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1);
    vector<int> R(n2);

    // Copy data
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = left;

    // Merge temporary arrays
    while (i < n1 && j < n2) {

        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        }
        else {
            arr[k++] = R[j++];
        }
    }

    // Copy remaining elements
    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }
}

// --------------------------------------------------
// Sequential Merge Sort
// --------------------------------------------------

void sequentialMergeSort(vector<int>& arr, int left, int right) {

    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    sequentialMergeSort(arr, left, mid);
    sequentialMergeSort(arr, mid + 1, right);

    merge(arr, left, mid, right);
}

// --------------------------------------------------
// Parallel Merge Sort using OpenMP
// --------------------------------------------------

void parallelMergeSort(vector<int>& arr, int left, int right) {

    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    // Create parallel tasks
    #pragma omp task shared(arr)
    parallelMergeSort(arr, left, mid);

    #pragma omp task shared(arr)
    parallelMergeSort(arr, mid + 1, right);

    // Wait for tasks to complete
    #pragma omp taskwait

    merge(arr, left, mid, right);
}

// --------------------------------------------------
// Utility Function
// --------------------------------------------------

void printArray(vector<int>& arr) {

    for (int num : arr)
        cout << num << " ";

    cout << endl;
}

// --------------------------------------------------
// Main Function
// --------------------------------------------------

int main() {

    int n = 100000;

    vector<int> arr1(n), arr2(n);

    // Generate random values
    for (int i = 0; i < n; i++) {

        int value = rand() % 100000;

        arr1[i] = value;
        arr2[i] = value;
    }

    // --------------------------------------------------
    // Sequential Merge Sort
    // --------------------------------------------------

    double start1 = omp_get_wtime();

    sequentialMergeSort(arr1, 0, n - 1);

    double end1 = omp_get_wtime();

    // --------------------------------------------------
    // Parallel Merge Sort
    // --------------------------------------------------

    double start2 = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            parallelMergeSort(arr2, 0, n - 1);
        }
    }

    double end2 = omp_get_wtime();

    // --------------------------------------------------

    cout << "Sequential Merge Sort Time: "
         << end1 - start1
         << " seconds\n";

    cout << "Parallel Merge Sort Time: "
         << end2 - start2
         << " seconds\n";

    return 0;
}
