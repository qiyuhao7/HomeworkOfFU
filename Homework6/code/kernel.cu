#include <math.h>
//CUDA RunTime API
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BLOCK_SIZE 16


__global__ static void matMultCUDA(const float *A, const float* B, float *C, int m, int n, int e)
{
	const int row = blockIdx.y * blockDim.y + threadIdx.y;
	const int col = blockIdx.x *blockDim.x + threadIdx.x;
	if (row < m && col < n)
	{
		float Cvalue = 0;
		for (int i = 0; i < e; i++)
		{
			Cvalue += A[row*e + i] * B[i*n + col];
		}
		C[n*row + col] = Cvalue;
	}


}


extern "C"
void mulWithCUDA(const float *A, const float *B, float *C, int m, int n, int e)
{
	cudaSetDevice(0);

	int heightA = m;
	int widthA = e;
	int  heightB = e;
	int widthB = n;
	int heightC = heightA;
	int widthC = widthB;

	float *d_A, *d_B, *d_C;

	cudaMalloc((void**)&d_A, sizeof(float)*heightA*widthA);
	cudaMalloc((void**)&d_B, sizeof(float)*heightB*widthB);
	cudaMalloc((void**)&d_C, sizeof(float)*heightC*widthC);

	cudaMemcpy(d_A, A, sizeof(float)*heightA*widthA, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, B, sizeof(float)*heightB*widthB, cudaMemcpyHostToDevice);

	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	//int numBlock = (widthB*heightA + BLOCK_SIZE*BLOCK_SIZE -1) / BLOCK_SIZE*BLOCK_SIZE;
	dim3 dimGrid(1, 1);
	dimGrid.x = ceil(float(widthB) / double(dimBlock.x));
	dimGrid.y = ceil(float(heightA) / double(dimBlock.y));
	//dim3 dimGrid((widthB + dimBlock.x - 1) / dimBlock.x, (heightA + dimBlock.y - 1) / dimBlock.y);
	//dim3 dimGrid((widthB ) / dimBlock.x, (heightA ) / dimBlock.y);
	matMultCUDA << <dimGrid, dimBlock >> > (d_A, d_B, d_C, heightC, widthC, widthA);

	cudaMemcpy(C, d_C, sizeof(float)*heightC*widthC, cudaMemcpyDeviceToHost);
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);


}