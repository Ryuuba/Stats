#include <iostream>
#include <cstdlib>
#include "statistics.h"


int main (int argc, char* argv[]) {
  unsigned sample_size = std::atoi(argv[1]);
  Statistics statistics(sample_size);
  statistics.compute_stats_sequential();
  statistics.compute_stats_concurrent();
  return 0;
}