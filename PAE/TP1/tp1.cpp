#include <chrono>
#include <x86intrin.h>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <cassert>

uint64_t rdtsc();
void ma_fonction(long long repetitions);

class EvalPerf{
private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_t;
  std::chrono::time_point<std::chrono::high_resolution_clock> end_t;
  uint64_t start_cycles;
  uint64_t end_cycles;
public:
    uint64_t nb_cycle(){
      return end_cycles - start_cycles ;
    }

    double  second(){
    std::chrono::duration<double> duration = end_t - start_t;
    return duration.count();
    }
    double milisecond(){
      return 1000 * second();
    };
    void start(){
      start_t = std::chrono::high_resolution_clock::now();
      start_cycles = rdtsc();
    };

    void stop(){
      end_t = std::chrono::high_resolution_clock::now();
      end_cycles = rdtsc();
    }
};


int main (int argc, char** argv){
  assert(argc==2);
  long long int repetitions = atoi(argv[1])*10000000;
  std::cout<< "nbr repetitions: " << repetitions << std::endl;

  EvalPerf PE;
  PE.start();
  ma_fonction(repetitions);
  PE.stop();

  std::cout<< "nbr seconds: " << PE.second() << std::endl;
  std::cout<< "nbr miliseconds: " << PE.milisecond() << std::endl;
  std::cout<< "nbr cycles: " << PE.nb_cycle() << std::endl;
  return 0;
  }


void ma_fonction(long long repetitions){
  int x = 0;
    for (int i = 0; i< repetitions; i++)
      x++;
}


uint64_t rdtsc(){
  unsigned int lo,hi;
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo ;
};