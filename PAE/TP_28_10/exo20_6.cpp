#include "eval-perf.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

void print_submatrix(const double* M, size_t m, size_t n, size_t s);

void MatrixTrans_obliv(size_t m,
                        size_t n,
                        const double *A,
                        size_t sa,
                        double *B,
                        size_t sb){
    if (m == 1 && n == 1){
/*         A = [A0 A1  ⇒B = AT =    [AT0 AT2
                A2 A3]               AT1 AT3] */
        //std::cout << "B[1] = A[sa] " << B[1] << " = " << A[sa] << std::endl;
        B[0] = A[0];
    }else{

        /* std::cout << "A0:"<< std::endl;
        print_submatrix(A, m/2, n/2, sa );
        std::cout << "A1:"<< std::endl;
        print_submatrix(A+n/2, m/2, n/2, sa );
        std::cout << "A2:"<< std::endl;
        print_submatrix(A+sa*(m/2), m/2, n/2, sa );
        std::cout << "A3:"<< std::endl;
        print_submatrix(A+n/2+sa*(m/2), m/2, n/2, sa ); */

        //B0 = AT0
        MatrixTrans_obliv(m/2, n/2, A, sa, B, sb);
        //B1 = AT2
        MatrixTrans_obliv(m/2, n/2, A+sa*(m/2), sa, B+n/2, sb);
        //B2 = AT1
        MatrixTrans_obliv(m/2, n/2, A+n/2, sa, B+sb*(m/2), sb);
        //B3 = AT3
        MatrixTrans_obliv(m/2, n/2, A+n/2+sa*(m/2), sa, B+sb*(m/2) + n/2, sb);
    }
}


void print_submatrix(const double* M, size_t m, size_t n, size_t s){
    for (size_t i = 0; i < m ; i++){
        for (size_t j = 0; j < n ; j++){
            std::cout << M[i*s+j]<< " ";
        }
        std::cout << std::endl;
    }
}

int main (int argc, char** argv){
    int n=16;
    double* A = new double[n*n];
    double* B = new double[n*n];
    //                  256
    for (size_t i = 0; i < 64 ; i++){
        A[i] = (i + 1);
    }
    
    for (size_t i = 0; i < 64 ; i++){
        B[i] = (double)88;
    }
    //print_submatrix(A, 8, 8, 16);
    
    //cas principal
    std::cout << "matrice carrée !" << std::endl;
    print_submatrix(A, 8, 8, 8);
    //print_submatrix(A, 2, 2, 8);
    MatrixTrans_obliv(8, 8, A, 8, B, 8);

/*     //si la matrice est de 4x6
    std::cout << "matrice 4x6 " << std::endl;
    print_submatrix(A, 4, 6, 6);
    MatrixTrans_obliv(4, 6, A, 6, B, 4); */

    print_submatrix(B, 8, 8, 8);
    //print_submatrix(A+16*8+8, 8, 8, 16);

    return 0;
    }


