#include "eval-perf.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>


void MatrixTrans_naif(std::vector<double> &A,
                      std::vector<double> &B,
                      int n){
  B.clear();
  B.resize(n*n);
  for (int i = 0; i< n; i++){
    for (int j = 0; j< n; j++){
      B[j + i*n] = A[i + j*n];
    }
  }
     
}

template<int b>
void MatrixTrans_bloc(std::vector<double> &A,
                      std::vector<double> &B,
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


int main (int argc, char** argv){
  assert(argc==2);
  //long long int repetitions = atoi(argv[1])*100000;
  long long int repetitions = 1000;
  //varier de 128 a 1024 en 2**k
  int k = atoi(argv[1]);
  int n = pow(2, k);
  std::cout << "k "<< k <<std::endl;
  //int blocs = 8;
  std::vector<double> B,
  A /*= {1, 1,1 ,1,
      2,2,2,2,
      3,3,3,3,
      4,4,4,4}*/;
 A.resize(n*n);
 //genere les matrices
  
  for (int i = 0; i< n; i++)
    for (int j = 0; j< n; j++)
      A[i + j*n] = j;

  
  EvalPerf PE;
  PE.start();
  

  for (int i = 0; i< repetitions; i++)
      MatrixTrans_naif(A,B,n);
  PE.stop();

  std::cout<< "MatrixTrans_naif " << std::endl;
  std::cout<< "nbr seconds: " << PE.second() << std::endl;
  std::cout<< "nbr milliseconds: " << PE.millisecond() << std::endl;
  std::cout<< "nbr cycles: " << PE.nb_cycle() << std::endl;
  std::cout<< "CPI: " << PE.CPI((double) repetitions) << std::endl;
  

 /* std::cout<< "A " << std::endl;
  for (int i = 0; i< n; i++){
    for (int j = 0; j< n; j++){
        std::cout << A[i + j*n] << " ";
      }
      std::cout << std::endl;
  } 
  std::cout<< "B " << std::endl;
  for (int i = 0; i< n; i++){
    for (int j = 0; j< n; j++){
      std::cout << B[i + j*n]  << " ";
    }
    std::cout << std::endl;
  }  
*/
  B.clear();
  B.resize(n*n);
   PE.start();
  

  for (int i = 0; i< repetitions; i++)
      MatrixTrans_bloc<8>(A,B,n);
  PE.stop();

  std::cout<< "MatrixTrans_bloc " << std::endl;
  std::cout<< "nbr seconds: " << PE.second() << std::endl;
  std::cout<< "nbr milliseconds: " << PE.millisecond() << std::endl;
  std::cout<< "nbr cycles: " << PE.nb_cycle() << std::endl;
  std::cout<< "CPI: " << PE.CPI((double) repetitions) << std::endl;

 /*
  std::cout<< "A " << std::endl;
  for (int i = 0; i< n; i++){
    for (int j = 0; j< n; j++){
        std::cout << A[i + j*n]  << " ";
      }
      std::cout << std::endl;
  } 
  std::cout<< "B " << std::endl;
  for (int i = 0; i< n; i++){
    for (int j = 0; j< n; j++){
      std::cout << B[i + j*n]  << " ";
    }
    std::cout << std::endl;
  }  */
  
  
  return 0;
  }


