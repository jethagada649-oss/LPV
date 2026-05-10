#include <iostream>
#include <cuda_runtime.h>

using namespace std;

#define N 3

// ======================================================
// VECTOR ADDITION KERNEL
// ======================================================

__global__ void vectorAdd(int *A, int *B, int *C, int size) {

    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < size) {
        C[i] = A[i] + B[i];
    }
}

// ======================================================
// MATRIX MULTIPLICATION KERNEL
// ======================================================

__global__ void matrixMultiply(int A[N][N],
                               int B[N][N],
                               int C[N][N]) {

    int row = threadIdx.y;
    int col = threadIdx.x;

    int sum = 0;

    for (int k = 0; k < N; k++) {
        sum += A[row][k] * B[k][col];
    }

    C[row][col] = sum;
}

// ======================================================
// MAIN FUNCTION
// ======================================================

int main() {

    // ==================================================
    // VECTOR ADDITION
    // ==================================================

    int size = 10;

    int h_A[size], h_B[size], h_C[size];

    // Initialize vectors
    for (int i = 0; i < size; i++) {
        h_A[i] = i;
        h_B[i] = i * 2;
    }

    int *d_A, *d_B, *d_C;

    // Allocate GPU memory
    cudaMalloc((void**)&d_A, size * sizeof(int));
    cudaMalloc((void**)&d_B, size * sizeof(int));
    cudaMalloc((void**)&d_C, size * sizeof(int));

    // Copy vectors to GPU
    cudaMemcpy(d_A, h_A,
               size * sizeof(int),
               cudaMemcpyHostToDevice);

    cudaMemcpy(d_B, h_B,
               size * sizeof(int),
               cudaMemcpyHostToDevice);

    // Launch kernel
    vectorAdd<<<1, size>>>(d_A, d_B, d_C, size);

    // Copy result back
    cudaMemcpy(h_C, d_C,
               size * sizeof(int),
               cudaMemcpyDeviceToHost);

    // Display vector addition result
    cout << "===== Vector Addition =====\n";

    for (int i = 0; i < size; i++) {

        cout << h_A[i]
             << " + "
             << h_B[i]
             << " = "
             << h_C[i]
             << endl;
    }

    // Free vector memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    // ==================================================
    // MATRIX MULTIPLICATION
    // ==================================================

    int h_M1[N][N] = {
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };

    int h_M2[N][N] = {
        {9,8,7},
        {6,5,4},
        {3,2,1}
    };

    int h_Result[N][N];

    int (*d_M1)[N], (*d_M2)[N], (*d_Result)[N];

    // Allocate GPU memory
    cudaMalloc((void**)&d_M1, sizeof(int)*N*N);
    cudaMalloc((void**)&d_M2, sizeof(int)*N*N);
    cudaMalloc((void**)&d_Result, sizeof(int)*N*N);

    // Copy matrices to GPU
    cudaMemcpy(d_M1, h_M1,
               sizeof(int)*N*N,
               cudaMemcpyHostToDevice);

    cudaMemcpy(d_M2, h_M2,
               sizeof(int)*N*N,
               cudaMemcpyHostToDevice);

    // Thread block configuration
    dim3 threadsPerBlock(N, N);

    // Launch matrix multiplication kernel
    matrixMultiply<<<1, threadsPerBlock>>>(d_M1,
                                           d_M2,
                                           d_Result);

    // Copy result back
    cudaMemcpy(h_Result, d_Result,
               sizeof(int)*N*N,
               cudaMemcpyDeviceToHost);

    // Display matrix multiplication result
    cout << "\n===== Matrix Multiplication =====\n";

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {

            cout << h_Result[i][j] << " ";
        }

        cout << endl;
    }

    // Free matrix memory
    cudaFree(d_M1);
    cudaFree(d_M2);
    cudaFree(d_Result);

    return 0;
}
