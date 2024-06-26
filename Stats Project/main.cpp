// main.cpp
// Project UID 5366c7e2b77742d5b2142097e51561a5
#include <iostream>
#include "stats.h"
#include "p1_library.h"
using namespace std;

int main() {
  string file_name;
  string column_name;
  cout << "enter a filename\n";
  cin >> file_name;
  cout << "enter a column name\n";
  cin >> column_name;
  cout << "reading column " << column_name << " from " << file_name << endl;
  vector<double> v = extract_column(file_name,column_name);
  cout << "Summary (value: frequency)" << endl;
  vector<pair<double,int>> summary = summarize(v);
  for(int i = 0; i < v.size(); ++i)
  {
    cout << summary[i].first << ":" << " " << summary[i].second << endl;
  }
  cout << endl;
  cout << "count = " << count(v) << endl;
  cout << "sum = " << sum(v) << endl;
  cout << "mean = " << mean(v) << endl;
  cout << "stdev = " << stdev(v) << endl;
  cout << "median = " << median(v) << endl;
  cout << "mode = " << mode(v) << endl;
  cout << "min = " << min(v) << endl;
  cout << "max = " << max(v) << endl;
  cout << "  0th percentile = " << percentile(v,0) << endl;
  cout << " 25th percentile = " << percentile(v, 25) << endl;
  cout << " 50th percentile = " << percentile(v,50) << endl;
  cout << " 75th percentile = " << percentile(v,75) << endl;
  cout << "100th percentile = " << percentile(v,100) << endl;
}