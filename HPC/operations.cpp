// to compile use : g++ -fopenmp operations.cpp
// to run use : ./a.out
#include <iostream>
#include <vector>
#include <cstdlib>
#include <climits>
#include <omp.h>

using namespace std;

int main() {

    int n = 10;

    vector<int> arr(n);

    // Generate random numbers
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }

    cout<<"Array is : \n";
    for(int i = 0 ; i < n ; i++)
      cout<<arr[i]<<" ";
    cout<<"\n";

    // ----------------------------------------
    // Variables for reduction
    // ----------------------------------------

    int minimum = INT_MAX;
    int maximum = INT_MIN;
    long long sum = 0;

    // ----------------------------------------
    // Start timing
    // ----------------------------------------

    double start = omp_get_wtime();

    // Parallel Reduction
    #pragma omp parallel for reduction(min:minimum) reduction(max:maximum) reduction(+:sum)
    for (int i = 0; i < n; i++) {

        if (arr[i] < minimum)
            minimum = arr[i];

        if (arr[i] > maximum)
            maximum = arr[i];

        sum += arr[i];
    }

    // Compute average
    double average = (double)sum / n;

    double end = omp_get_wtime();

    // ----------------------------------------
    // Display results
    // ----------------------------------------

    cout << "Minimum Value : " << minimum << endl;

    cout << "Maximum Value : " << maximum << endl;

    cout << "Sum            : " << sum << endl;

    cout << "Average        : " << average << endl;

    cout << "Execution Time : "
         << end - start
         << " seconds" << endl;

    return 0;
}
