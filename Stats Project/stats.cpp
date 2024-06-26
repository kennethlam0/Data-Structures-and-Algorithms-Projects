// stats.cpp 
// Project UID 5366c7e2b77742d5b2142097e51561a5
#include "stats.h"
#include <cassert>
#include <cmath>
#include <vector>
#include "p1_library.h"
#include <iostream>

using namespace std;

vector<pair<double,int>> summarize(vector<double> v)
{
    sort(v);
    vector<pair<double,int>> freq;
    for(int i = 0; i < v.size(); ++i)
    {
        int count = 1; 
        while(i < v.size()-1 && v[i] == v[i + 1])
        {
            ++i;
            ++count;
        }
        freq.push_back(make_pair(v[i],count));
    }
    return freq;
}


int count(vector<double> v)
{
    return v.size();
}

double sum(vector<double> v)
{
    double total = 0;
    for(int i = 0; i < v.size(); ++i)
    {
        total += v[i];
    }
    return total;   
}

double mean(vector<double> v)
{
    return sum(v) / v.size();
}

double median(vector<double> v)
{
    sort(v);
    int n = v.size();
    if(n % 2 == 0)
    {
        return (v[n / 2 - 1] + v[n / 2]) / 2;
    }
    else
    {
        return v[n / 2];
    }
}

double mode(vector<double> v)
{
    sort(v);
    double current_number = v[0];
    double current_number_count = 0; 
    double maxcount = 0;
    double mode = current_number;
    
    for(auto number: v)
    {
        if(number == current_number)
        {
            ++current_number_count;
        }
        else
        {
            if(current_number_count > maxcount)
            {
                maxcount = current_number_count;
                mode = current_number;
            }
            current_number = number;
            current_number_count = 1;
        }
    }   

    if(current_number_count > maxcount)
    {
        mode = current_number;
    }
    return mode;
}
	
double min(vector<double> v)
{
    double minimum = v[0];
    for(int i = 0; i < v.size(); ++i)
    {
        if(v[i] < minimum)
        {
            minimum = v[i];
        }
    }
    return minimum;
}

double max(vector<double> v)
{
    double maximum = v[0];
    for(int i = 0; i < v.size(); ++i)
    {
        if(v[i] > maximum)
        {
            maximum = v[i];
        }
    }
    return maximum; 
}

double stdev(vector<double> v)
{
    double total = 0;
    for(int i = 0; i < v.size(); ++i)
    {
        total += pow(v[i] - mean(v),2);
    }
    double stdev = sqrt(total / (v.size() - 1));
    return stdev;
}

double percentile(vector<double> v, double p)
{
    double rank = (p / 100) * (v.size() - 1) + 1;
    double intPart;
    double fracPart;
    fracPart = modf(rank,&intPart);
    double ans = v[intPart - 1] + fracPart * (v[intPart] - v[intPart - 1]);
    return ans; 
}