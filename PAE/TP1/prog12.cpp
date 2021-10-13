#include "eval-perf.h"
#include <iostream>
#include <immintrin.h>
#include <vector>



//using namespace std;

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
/* void produitMat1( double *A, int m, int K,
                double *B, int n,
                double *C){
    //C.resize(m*n,(double) 0);
    C = new double[m*n];
    for (int i=0;i<m*n;i++){
        C[i]=0;
    }
    std::cout<<"TEST 18: "<< std::endl;
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            for (int k = 0; k < K; k++){
                C[i*m + j] += A[i*K + k] * B[j + k*n];
            }
            std::cout<<"TEST after: "<< C[i*m + j]<< std::endl;
        }
    }
} */
/* double min(float *x, int n) {
    __m256 x_vec, min, mask;
    min = _mm256_load_ps(x);

    cout<<"TEST avant boucle: " << endl;
    for(int i = 1; i < n ; i += 8){
        cout<<"TEST 16" << endl;
        x_vec = _mm256_load_ps(x+i);
        cout<<"TEST 17: " << endl;
        mask = _mm256_cmp_ps(x_vec, min, _CMP_LT_OQ);
        min = _mm256_blendv_ps(x_vec, min, mask);
    }
    
    float res[8],somme = 0;
    _mm256_storeu_ps(res, min);
    cout << "res[0] : " << res[0] << " res[1] : " << res[1] << " res[2] : " << res[2] << " res[3] : " << res[3] << endl;
    
    // for (int i = 0; i < 8 ;i++ )
    //     somme += res[i];
    return  somme ;
} */


double* produitMat1SIMD(double *A, int m, int K,
                    double *B, int n){
    double c_ij = 0;
    C = new double[m*n];
    for (int i=0;i<m*n;i++){
        C[i]=0;
    }
    std::cout<<"TEST 18: "<< std::endl;
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            for (int k = 0; k < K; k++){
                C[i*m + j] += A[i*K + k] * B[j + k*n];
            }
            std::cout<<"TEST after: "<< C[i*m + j]<< std::endl;
        }
    }
    return &c_ij;
}

int main(int argc, char** argv) {
    int size = 16;        // taille du vect
    int nbr_calls = 123456; //nbr d'appels des fonctions
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

    EvalPerf PE;
    std::cout<<"TEST min: "<< std::endl;
    produitMat1(A, m, k, B, n, C);


    for(int i = 0; i < m*n; i ++) {
        std::cout<<"TEST min: "<< C[i] << std::endl;

    } 
    std::vector<double> A1,B1;
    //[1 2 3]
    A = {1,2,3};
    //[1
    // 2   
    // 3]
    /* produitMat1(A, 1, 3, A, 1, C);
    std::cout<<"res: "<< C[0] << std::endl; */

    //std::cout<<"TEST min: "<< C << std::endl;

    // PE.start();
    // for(int i = 0; i < nbr_calls; i ++) {
    //     reduce(v_test, result);
    // }
    // PE.stop();

    // cout<<"TEST reduce: "<<std::endl;
    // cout<<"nombre d'instructions total: "<<size * nbr_calls<<std::endl;
    // cout<<"nbr cycles: "  <<PE.nb_cycle()<<std::endl;
    // cout<<"Temps d'executions (par appel de fonction): "<<std::endl;
    // cout<<"nbr secondes: "  <<PE.second()<<std::endl;
    // cout<<"nbr millisecondes: " <<PE.millisecond()<<std::endl;
    // cout<<"CPI= "<<PE.CPI(size * nbr_calls)<<std::endl;
    // cout<<"Temps d'exécution total:"<<PE.second()<<std::endl;
    // cout<<std::endl;


    

    

    return 0;
}