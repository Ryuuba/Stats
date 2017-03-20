#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <thread>
#include <algorithm>
#include <cmath>
#include <unordered_map>

using std::thread;
using std::vector;
using std::chrono::system_clock;
using std::chrono::microseconds;
using std::chrono::duration_cast;
using std::unordered_map;
using std::cout;
using std::endl;

class Statistics {
private:
  double mean, variance, mode, median;
  vector<double> sample;
private:
  void compute_mean();
  void compute_variance();
  void compute_mode();
  void compute_median();
public:
  Statistics(unsigned);
  void compute_stats_sequential();
  void compute_stats_concurrent();
};