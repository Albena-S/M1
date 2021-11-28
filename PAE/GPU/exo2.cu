#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <assert.h>

#define cudaCheckError() {                                                                       \
        cudaError_t e=cudaGetLastError();                                                        \
        if(e!=cudaSuccess) {                                                                     \
            printf("Cuda failure %s:%d: '%s'\n",__FILE__,__LINE__,cudaGetErrorString(e));        \
            exit(EXIT_FAILURE);                                                                  \
        }                                                                                        \
    }

__global__ void vecAddKernel(float *A,
                             float *B,
                             float *C,
                             int n){
  int i=blockIdx.x*blockDim.x+threadIdx.x;
  if (i < n) C[i]=A[i] + B[i];
}

__global__ void vecAdd2DKernel(float *A,
                             float *B,
                             float *C,
                             int x,
			                       int y){
  int i=blockIdx.x*blockDim.x+threadIdx.x;
  int j=blockIdx.y*blockDim.y+threadIdx.y; 	
  if (i < x && j < y) C[i*x + j]= A[i*x + j] + B[i*x + j];//C[j*y +i]= A[j*y +i] + B[j*y +i];
}

__host__ void vecAdd(float *h_A,
                       float *h_B,
                       float *h_C,
                       int n) {
  int size = n * sizeof(float);
  float *d_A, *d_B, *d_C;

  cudaMalloc((void **) &d_A, size);
  cudaMalloc((void **) &d_B, size);
  cudaMalloc((void **) &d_C, size);

  cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice );
  cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice );

  
  int threads=128;
  int blocks=(n+threads-1)/threads;


  vecAddKernel<<<blocks,threads>>>(d_A, d_B, d_C, n);

  cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost );
  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);
}

__host__ void vecAdd2D(float *h_A,
                       float *h_B,
                       float *h_C,
                       int x,
		                   int y) {
  int size = x*y * sizeof(float);
  float *d_A, *d_B, *d_C;

  cudaMalloc((void **) &d_A, size);
  cudaMalloc((void **) &d_B, size);
  cudaMalloc((void **) &d_C, size);

  cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice );
  cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice );


  dim3 DimGrid((x-1)/16 + 1, (y-1)/16 + 1, 1);
  dim3 DimBlock(16, 16, 1);
  //le max c'est 1024 donc 32x32 mais c'est mieux d'utiliser 16x16
  //1. decision du nombre de threads donc dimblock
  //2.divise par le nombre de threads pour avoir le nombre de blocs donc dimGrid
   // nvcc deviceQuery.cpp -I/usr/local/cuda-11.4/samples/common/inc
   // ./a.out 


  //Maximum number of threads per multiprocessor:  2048
  //Maximum number of threads per block:           1024



  vecAdd2DKernel<<<DimGrid,DimBlock>>>(d_A, d_B, d_C, x, y);

  cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost );
  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);
}


void vecAddClassique(float *A,
                     float *B,
                     float *C,
                     int n){
  for (int i = 0; i < n; ++i) C[i]=A[i] + B[i];
}



int main(int argc, char** argv){
/*assert(argc==2);
  int n = atoi(argv[1]);
*/
/*  
  //2.5

  int n = 1024;
  float *h_A = new float[n];
  float *h_B = new float[n];
  float *h_C = new float[n]; 

  for (int i = 0; i < n; ++i){
    h_A[i] = i;
  }

  vecAdd(h_A, h_B, h_C, n); 

  for (int i = 0; i < n ; i++)
    std::cout << h_C[i] << " ";

  cudaCheckError();*/

  /*
  //2.6
  float *h_A = new float[n];
  float *h_B = new float[n];
  float *h_C = new float[n]; 

  for (int i = 0; i < n; ++i){
    h_A[i] = rand() % 65535;;
  }

  vecAddClassique(h_A, h_B, h_C, n); 

  for (int i = 0; i < n ; i++)
    std::cout << h_C[i] << " ";
  std::cout << std::endl;
  */
  //2.9
  int x = 32,y=32 ; 
  float *h_A = new float[x*y];
  float *h_B = new float[x*y];
  float *h_C = new float[x*y]; 


  for (int i = 0; i < x*y; ++i){
    h_A[i] = rand() % 65535;
    //std::cout << h_A[i] << " ";
  }
  std::cout << std::endl;
  

  vecAdd2D(h_A, h_B, h_C, x, y); 

  for (int i = 0; i < x*y ; i++)
    std::cout << h_C[i] << " ";
  std::cout << std::endl;


  return 0;
}
