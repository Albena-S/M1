#include "eval-perf.h"
#include <iostream>
#include <unistd.h>
#include <cassert>
#define REPETITIONS 100000


void sommePrefixe(int* list, int size);
void ma_fonction(long long repetitions);

int main (int argc, char** argv){
  //assert(argc==2);
  //int repetitions = atoi(argv[1]) ;
  long long int size = 10000;
  int* list = new int[size];

  for (int i = 0; i <size; i++){
    list[i] = 1;
  }

  EvalPerf PE;
  PE.start();

  int x = 0;
    for (int i = 0; i< REPETITIONS; i++)
      sommePrefixe(list,size);
  
  PE.stop();

  
  
  std::cout<< "nbr seconds: " << PE.second() << std::endl;
  std::cout<< "nbr milliseconds: " << PE.millisecond() << std::endl;
  std::cout<< "nbr cycles: " << PE.nb_cycle() << std::endl;
  std::cout<< "CPI: " << PE.CPI((double) size) << std::endl;
  
  delete[] list;
  return 0;
  }


void sommePrefixe(int* list, int size){
  int acc = list[0];
  for (int i = 1; i<size; i++){
    acc+= list[i];
    list[i] = acc;
  }
}


/** g++ prog.cpp eval-perf.cpp -o prog
 * 
 **/
