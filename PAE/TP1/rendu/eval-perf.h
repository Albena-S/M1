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
    uint64_t nb_cycle();
    double  second();
    double millisecond();
    void start();
    void stop();
    double CPI(int n);
};

