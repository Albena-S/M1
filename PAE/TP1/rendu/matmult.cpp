#include "eval-perf.h"
#include <iostream>
#include <immintrin.h>
#include <vector>
#include <float.h>
#define REPETITIONS 100000



//A : m x k
//B : k x n
//C : m x n
void produitMat1(std::vector<double> &A, int m, int K,
                std::vector<double> &B, int n,
                std::vector<double> &C){
    double c_ij = 0;
    C.resize(m*n,(double) 0);
    std::cout<<"TEST 18: "<< std::endl;
    for (int i = 0; i < m; i++){
        c_ij = 0;
        for (int j = 0; j < n; j++){
            for (int k = 0; k < K; k++){
                C[i*m + j] += A[i*K + k] * B[j + k*n];
            }
        }
    }
}

void matmult(double *C, double *A, double *B, size_t n){
    /*__m256 A1, B1,B2,B3, B4, 
            mask12 = _mm256_set_pd((double) 0,(double) DBL_MAX,(double) 0,(double) 0),
            mask34 = _mm256_set_pd(0,0,0,DBL_MAX),
            mask = _mm256_set1_pd(0,0,DBL_MAX,DBL_MAX);

    for (int i = 0; i < (n-4); i++){
        for (int j = 0; j < n; j++){
            for (int k = 0; k < n; k+=4){
                //charger la colonne de B
                B1 =  _mm256_load_pd(B+i);
                B2 =  _mm256_load_pd(B+i+n+1);
                B1 = _mm256_blendv_pd(B1,B2,mask12);
                B3 =  _mm256_load_pd(B+i+2*n+2);
                B4 =  _mm256_load_pd(B+i+3*n+3);
                B3 = _mm256_blendv_pd(B1,B2,mask34);
                B1 = _mm256_blendv_pd(B1,B3,mask);

                //charger la ligne de A
                A1 = _mm256_load_pd(A+i);

                //C[i*m + j] += A[i*K + k] * B[j + k*n];
            }
        }
    }*/
}


int main(int argc, char** argv) {
    std::vector<double> A,B,C;
    int m,k,n;
    //[1 2 3
    // 4 5 6]
    A = {1,2,3,4,5,6};
    m = 2;
    k = 3;
    //[10 11
    // 20 21
    // 30 31]
    B = {10 , 11, 20, 21 , 30, 31};
    n = 2;

    
    //produitMat1(A, m, k, B, n, C);


  
    EvalPerf PE;
    PE.start();

    int x = 0;
        for (int i = 0; i< REPETITIONS; i++){

        }

      
      PE.stop();

      
      
      std::cout<< "nbr seconds: " << PE.second() << std::endl;
      std::cout<< "nbr milliseconds: " << PE.millisecond() << std::endl;
      std::cout<< "nbr cycles: " << PE.nb_cycle() << std::endl;
      //std::cout<< "CPI: " << PE.CPI((double) size) << std::endl;
        

    return 0;
}


/*g++ matmult.cpp eval-perf.cpp -o matmult*/