#include "eval-perf.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

EvalPerf PE;

void print_submatrix(const double* M, size_t m, size_t n, size_t s);


void MatrixTrans_naif(const double* A,
                      double* B,
                      int n){
  for (int i = 0; i< n; i++){
    for (int j = 0; j< n; j++){
      B[j + i*n] = A[i + j*n];
    }
  }
     
}

template<int b>
void MatrixTrans_bloc(const double* A,
                      double* B,
                      int n){

  for (int i = 0; i< n; i+=b){
    for (int j = 0; j< n; j+=b){
      for (int k = i; k< i+b; k++){
        for (int l = j; l< j + b; l++){
            B[l + k*n] = A[k + l*n];
        }
      }
    }
  } 
}


void MatrixTrans_obliv_v1(size_t m,
                        size_t n,
                        const double *A,
                        size_t sa,
                        double *B,
                        size_t sb){
    if (m == 1 && n == 1){
/*         A = [A0 A1  ⇒B = AT =    [AT0 AT2
                A2 A3]               AT1 AT3] */
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
        MatrixTrans_obliv_v1(m/2, n/2, A, sa, B, sb);
        //B1 = AT2
        MatrixTrans_obliv_v1(m/2, n/2, A+sa*(m/2), sa, B+n/2, sb);
        //B2 = AT1
        MatrixTrans_obliv_v1(m/2, n/2, A+n/2, sa, B+sb*(m/2), sb);
        //B3 = AT3
        MatrixTrans_obliv_v1(m/2, n/2, A+n/2+sa*(m/2), sa, B+sb*(m/2) + n/2, sb);
    }
}

template<int min_appels>
void MatrixTrans_obliv_v2(size_t m,
                        size_t n,
                        const double *A,
                        size_t sa,
                        double *B,
                        size_t sb){
    if (n <= min_appels){
        MatrixTrans_naif(A, B, n);
    }else{
        //B0 = AT0
        MatrixTrans_obliv_v2<min_appels>(m/2, n/2, A, sa, B, sb);
        //B1 = AT2
        MatrixTrans_obliv_v2<min_appels>(m/2, n/2, A+sa*(m/2), sa, B+n/2, sb);
        //B2 = AT1
        MatrixTrans_obliv_v2<min_appels>(m/2, n/2, A+n/2, sa, B+sb*(m/2), sb);
        //B3 = AT3
        MatrixTrans_obliv_v2<min_appels>(m/2, n/2, A+n/2+sa*(m/2), sa, B+sb*(m/2) + n/2, sb);
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
void print_PE(std::string method_name, int repetitions){
  std::cout<<  method_name << std::endl;
  std::cout<< "nbr seconds: " << PE.second() << std::endl;
  std::cout<< "nbr milliseconds: " << PE.millisecond() << std::endl;
  std::cout<< "nbr cycles: " << PE.nb_cycle() << std::endl;
  std::cout<< "CPI: " << PE.CPI((double) repetitions) << std::endl;


}

int main (int argc, char** argv){
  assert(argc==2);
  //long long int repetitions = atoi(argv[1])*100000;
  long long int repetitions = 1000;
  //varier de 128 a 1024 en 2**k
  int k = atoi(argv[1]);
  int n = pow(2, k);
  std::cout << "k "<< k <<std::endl;
  //int blocs = 8;
  //double A[n*n], B[n*n];
  double* A = new double[n*n];
  double* B = new double[n*n];

 //genere les matrices
  
  for (int i = 0; i< n; i++)
    for (int j = 0; j< n; j++)
      A[i + j*n] = j;

  

  PE.start();
  for (int i = 0; i< repetitions; i++)
      MatrixTrans_naif(A,B,n);
  PE.stop();

  print_PE("MatrixTrans_naif ", repetitions);

  PE.start();
  for (int i = 0; i< repetitions; i++)
      MatrixTrans_bloc<8>(A,B,n);
  PE.stop();

  print_PE("MatrixTrans_bloc ", repetitions);


  PE.start();
  for (int i = 0; i< repetitions; i++)
      MatrixTrans_obliv_v1(n, n, A, n, B, n);
  PE.stop();

  print_PE("MatrixTrans_obliv_v1 ", repetitions);
  
  MatrixTrans_obliv_v2<9>(n, n, A, n, B, n);
  PE.start();
  for (int i = 0; i< repetitions; i++)
      MatrixTrans_obliv_v2<8>(n, n, A, n, B, n);
  PE.stop();

  print_PE("MatrixTrans_obliv_v2 ", repetitions);

  return 0;
  }


