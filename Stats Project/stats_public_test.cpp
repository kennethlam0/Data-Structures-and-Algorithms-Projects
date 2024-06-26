// Project UID 5366c7e2b77742d5b2142097e51561a5
// THIS TEST CASE WILL ONLY TELL YOU IF YOUR CODE COMPILES.
// YOU NEED TO WRITE YOUR OWN COMPREHENSIVE TEST CASES IN stats_tests.cpp

#include "stats.h"
#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

int main()
{
  vector<double> data={1,1,1,2,2};
  cout << "count" << count(data) << endl;
  cout << "sum" << sum(data) << endl;
  cout << "mean" << mean(data) << endl;
  cout << "median" << median(data) << endl;
  cout << "mode" << mode(data) << endl;
  cout << "min" << min(data) << endl;
  cout << "max" << max(data) << endl;
  cout << "stdev" << stdev(data) << endl;
  cout << "percentile" << percentile(data, 40) << endl;

  summarize(data);

  return 0;
}

