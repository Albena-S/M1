#include "eval-perf.h"

uint64_t rdtsc();

uint64_t EvalPerf::nb_cycle(){
  return end_cycles - start_cycles ;
}

double  EvalPerf::second(){
  std::chrono::duration<double> duration = end_t - start_t;
  return duration.count();
  }
double EvalPerf::millisecond(){
  return 1000 * second();
};
void EvalPerf::start(){
  start_t = std::chrono::high_resolution_clock::now();
  start_cycles = rdtsc();
};

void EvalPerf::stop(){
  end_t = std::chrono::high_resolution_clock::now();
  end_cycles = rdtsc();
}

double EvalPerf::CPI(int n){
  return (double)(end_cycles - start_cycles ) / (double) n;
}

uint64_t rdtsc(){
  unsigned int lo,hi;
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo ;
};