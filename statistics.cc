#include "statistics.h"

Statistics::Statistics(unsigned sample_size){
  //Random Number Generator
  std::default_random_engine rng;
  //Object producing normal-distributed numbers
  std::normal_distribution<double> normal(1.8,0.07);
  //Saves samples in the sample vector. Samples are in the format i.dd, 
  //e.g. 1.85 
  for (int i = 0; i < sample_size; i++)
    sample.push_back(std::round(normal(rng)*100) / 100);
  //Here the sample vector is sorted in order to compute the median
  std::sort(sample.begin(), sample.end());
  mean = 0.0;
  median = 0.0;
  variance = 0.0;
  mode = 0.0;
}

void Statistics::compute_stats_sequential() {
  auto start = system_clock::now();
  compute_mean();
  compute_variance();
  compute_mode();
  compute_median();

  cout << "Mean: " << mean << endl
       << "Variance: " << variance << endl
       << "Median: " << median << endl
       << "Mode: " << mode << endl;

  auto elapsed = system_clock::now() - start;
  cout << "Sequential time: " 
       << duration_cast<microseconds>(elapsed).count()
       << " us " << endl;
}

void Statistics::compute_stats_concurrent() {

  auto start = system_clock::now();
  //Four threads are spawn to concurrently compute the mean, the media, the 
  //mode, and the variance
  thread thmean(compute_mean, (*this));
  thread thmedian(compute_median, (*this));
  thread thmode(compute_mode, (*this));
  thread thvariance(compute_variance, (*this));
  //Don't forget join the threads
  thmean.join();
  thmedian.join();
  thmode.join();
  thvariance.join();

  cout << "Mean: " << mean << endl
       << "Variance: " << variance << endl
       << "Median: " << median << endl
       << "Mode: " << mode << endl;

  auto elapsed = system_clock::now() - start;
  cout << "Concurrent time: " 
       << duration_cast<microseconds>(elapsed).count()
       << " us " << endl;
}

void Statistics::compute_mean () {
  for (auto& num : sample)
    mean += num;
  mean /= sample.size();
}

void Statistics::compute_variance () {
  double sum = 0.0, suusq = 0.0;
  for (auto& num : sample) {
    suusq += std::pow(num, 2);
    sum += num;
  }
  variance = (suusq - std::pow(sum, 2) / sample.size()) / (sample.size() - 1);
}

void Statistics::compute_median () {
  unsigned middle = sample.size() / 2;
  median = sample[middle];
}

void Statistics::compute_mode () {
  //Map storing pairs <heigth, frequency>
  std::unordered_map<double, unsigned> freq_table;
  //This loop is used to obtain the frecuency of each height in sample
  for (auto& element : sample) {
    if (freq_table.find(element) != freq_table.end())
      freq_table[element]++;//Increases the frequency of an element
    else
      freq_table[element] = 1;//Initializes the frecuency of an element
  }
  //Mode is the value with the maximum frequency in the freq_table. To find this
  //value, the max_element function is used. It receives three arguments: an
  //iterator pointing to the begin of a data structure, an iterator pointing to
  //the end of the data structure, and a function to compare the values,
  //optionally. It returns an iterator pointing to the max element. Recall that
  //elements in a map are pairs, so you need only the first element to get the
  //mode. The second element's the frequency.
  mode = std::max_element(
    freq_table.begin(), //The map starts here
    freq_table.end(), //This is the end of the map
    //This lambda function is used to compare frequencies in the freq_map
    [](std::pair<double, unsigned> lhs, std::pair<double, unsigned> rhs) {
      return lhs.second < rhs.second;
    } 
    )->first; //This is the first element in of the pair <height, frequency>,
    //i.e. the height
}
