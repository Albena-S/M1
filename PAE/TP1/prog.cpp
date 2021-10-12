#include "eval-perf.h"
#include <iostream>
#include <unistd.h>
#include <cassert>


void ma_fonction(long long repetitions);

int main (int argc, char** argv){
  assert(argc==2);
  long long int repetitions = atoi(argv[1])*10000000;
  std::cout<< "nbr repetitions: " << repetitions << std::endl;

  EvalPerf PE;
  PE.start();
  ma_fonction(repetitions);
  PE.stop();

  std::cout<< "nbr seconds: " << PE.second() << std::endl;
  std::cout<< "nbr milliseconds: " << PE.millisecond() << std::endl;
  std::cout<< "nbr cycles: " << PE.nb_cycle() << std::endl;
  std::cout<< "CPI: " << PE.CPI((double) repetitions) << std::endl;
  return 0;
  }


void ma_fonction(long long repetitions){
  int x = 0;
    for (int i = 0; i< repetitions; i++)
      x++;
}


/** g++ prog.cpp eval-perf.cpp -o prog
 * 
 **/
