/* stats_tests.cpp
 *
 * Unit tests for the simple statistics library
 * Project UID 5366c7e2b77742d5b2142097e51561a5
 *
 * EECS 280 Project 1
 *
 * Protip #1: Write tests for the functions BEFORE you implement them!  For
 * example, write tests for median() first, and then write median().  It sounds
 * like a pain, but it helps make sure that you are never under the illusion
 * that your code works when it's actually full of bugs.
 *
 * Protip #2: Instead of putting all your tests in main(),  put each test case
 * in a function!
 */


#include "stats.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
using namespace std;

void test_sum_small_data_set();
void test_count_small_data_set();
void test_mean_small_data_set();
void test_median_small_data_set();
void test_mode_small_data_set();
void test_min_small_data_set();
void test_max_small_data_set();
void test_summarize_small_data_set();
void test_stdev_small_data_set();
void test_percentile_small_data_set();
// Add prototypes for you test functions here.

int main() 
{
  test_sum_small_data_set();
  test_count_small_data_set();
  test_mean_small_data_set();
  test_median_small_data_set();
  test_mode_small_data_set();
  test_min_small_data_set();
  test_max_small_data_set();
  test_summarize_small_data_set();
  test_stdev_small_data_set();
  test_percentile_small_data_set();
  return 0;
}

void test_sum_small_data_set() 
{
  cout << "test_sum_small_data_set" << endl;

  vector<double> data;
  data.push_back(1);
  data.push_back(2);
  data.push_back(3);

  assert(sum(data) == 6);

  cout << " sum PASS!" << endl;
}

void test_count_small_data_set()
{
  cout << "test_count" << endl;
  vector<double> data = {9, 0, 3, 1, 7, 0, 3, 0, 8, 3};
  assert(count(data) == data.size());
  cout << "count PASS!" << endl;
}

void test_mean_small_data_set()
{
  cout << "test_mean" << endl;
  vector<double> data = {9, 0, 3, 1, 7, 0, 3, 0, 8, 3};
  assert(mean(data) == 3.4);
  cout << "mean PASS!" << endl;
} 

void test_median_small_data_set()
{
  cout << "test_mean" << endl;
  vector<double> data = {9, 0, 3, 1, 7, 0, 3, 0, 8, 3};
  assert(median(data) == 3);
  cout << "median PASS!" << endl;
}

void test_mode_small_data_set()
{
  cout << "test_mode" << endl;
  vector<double> data = {1,1,1,2,2,2};
  assert(mode(data) == 1);
  vector<double> data2 = {1,2,1,2,1,2,2};
  assert(mode(data2) == 2);
  vector<double> data3 = {1};
  assert(mode(data3) == 1);
  cout << "mode PASS!" << endl;
}

void test_min_small_data_set(){
  cout << "test_min" << endl;
  vector<double> data = {9, 0, 3, 1, 7, 0, 3, 0, 8, 3};
  assert(min(data) == 0);
  cout << "min PASS!" << endl;
}

void test_max_small_data_set(){
  cout << "test_max" << endl;
  vector<double> data = {9, 0, 3, 1, 7, 0, 3, 0, 8, 3};
  assert(max(data) == 9);
  cout << "max PASS!" << endl;
}

void test_summarize_small_data_set(){
  cout << "test_summarize" << endl;
  vector<double> data = {1,2,2,3,3,3};
  vector<pair<double,int>> myvec = summarize(data);
  assert(myvec[0].first == 1);
  assert(myvec[0].second == 1);
  assert(myvec[1].first == 2);
  assert(myvec[1].second == 2);
  assert(myvec[2].first == 3);
  assert(myvec[2].second == 3);
  cout << "summarize PASS!" << endl;
 }

void test_stdev_small_data_set()
{
  cout << "test_stdev" << endl;
  vector<double> data = {1,2,3};
  assert(stdev(data) == 1);
  cout << "stdev PASS!" << endl;
}

void test_percentile_small_data_set()
{
  cout << "test_percentile" <<endl;
  vector<double> data = {15,20,35,40,50};
  assert(percentile(data,40) == 29);
  cout << "percentile PASS!" << endl;

}
// Add the test function implementations here.
