#include "eval-perf.h"
#include <iostream>
#include <unistd.h>
#include <vector>
#include <cassert>
#define REPETITIONS 100000000
#define ALPHA 2

int polynome1(std::vector<int> &coef, int alpha, int degree);
int polynome2(std::vector<int> &coef, int alpha, int degree);

int main (int argc, char** argv){
  assert(argc=2);
  int degree = atoi(argv[1]) ;
  std::vector< int > coef(degree+1);

  for (int i = 0; i <degree+1; i++){
    coef[i]= 1;
  }

  EvalPerf PE;
  PE.start();
  std::cout<< "polynome1 :" << polynome1(coef, ALPHA, degree) << std::endl;
  for (int i = 0; i< REPETITIONS; i++)
      polynome1(coef, ALPHA, degree);
  
  PE.stop();

  
  
  std::cout<< "nbr seconds: " << PE.second() << std::endl;
  std::cout<< "nbr milliseconds: " << PE.millisecond() << std::endl;
  std::cout<< "nbr cycles: " << PE.nb_cycle() << std::endl;
  std::cout<< "CPI: " << PE.CPI((double) coef.size()*3) << std::endl;


  PE.start();
  std::cout<< "polynome2 :" << polynome2(coef, ALPHA, degree) << std::endl;
  for (int i = 0; i< REPETITIONS; i++)
      polynome2(coef, ALPHA, degree);
  
  PE.stop();

  
  
  std::cout<< "nbr seconds: " << PE.second() << std::endl;
  std::cout<< "nbr milliseconds: " << PE.millisecond() << std::endl;
  std::cout<< "nbr cycles: " << PE.nb_cycle() << std::endl;
  std::cout<< "CPI: " << PE.CPI((double) coef.size()*2) << std::endl;
  
  return 0;
  }


int polynome1(std::vector<int> &coef, int alpha, int degree){
  int res = 0;
  int alpha_pow = 1;
  int size = coef.size();
  for (int i = 0; i< size; i++){
    res+= coef[i] * alpha_pow;
    alpha_pow = alpha_pow * alpha;
  }
  return res;
}

int polynome2(std::vector<int> &coef, int alpha, int degree){
  int res = 0;
  int size = coef.size();
  for (int i = size-1 ; i>= 0; i--){
    res = coef[i] + alpha*res;
  }
  return res;
  // p a = 2 + 2*a + 2* a**2 
  // p a = 2 + a(2 + a(2+0))

}


/** g++ prog.cpp eval-perf.cpp -o prog
 * 
 **/
