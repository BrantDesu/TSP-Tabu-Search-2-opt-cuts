#include <iostream>
#include <fstream>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <limits>
#include <sstream>
#include <iomanip>
using namespace std;

vector<int> slicing(vector<int>& arr, int X, int Y) 
{ 
    // Starting and Ending iterators 
    auto start = arr.begin() + X; 
    auto end = arr.begin() + Y + 1; 
    if (X < 0) start = arr.begin();
    if (Y < 0) end = arr.end(); 
    // To store the sliced vector 
    vector<int> result(end - start); 
    // Copy vector using copy function() 
    copy(start, end, result.begin()); 
    // Return the final sliced vector 
    return result; 
}
vector<double> makeIntervalArray(double min, double max, int cant)
{
	vector<double> arr;
	double num = (max - min)/(cant - 1);
	arr.push_back(min);
	for (int i = 1; i < cant-1; ++i)
	{
		arr.push_back(min + num * i);
	}
	arr.push_back(max);
	return arr;
}
int main(int argc, char const *argv[])
{
	vector<int> slice1, slice2, slice3, actualTour = {1,2,3,4,5,6,1};
	int n = (int)actualTour.size() -1;

	cout << "\nTour inicial: " ;
	for(auto num:actualTour) cout << num << " ";
	cout << endl << endl; 

	for (int i = 0; i < n-2; ++i)
	{
		for (int j = i+2; j < n && j-i != n - 1; ++j)
		{

			slice1 = slicing(actualTour, -1, i);
			slice2 = slicing(actualTour, i+1, j);
			slice3 = slicing(actualTour, j+1, -1);

			reverse(slice2.begin(), slice2.end());

			slice1.insert( slice1.end(), slice2.begin(), slice2.end() );
			slice1.insert( slice1.end(), slice3.begin(), slice3.end() );

			cout << "cortes en arcos: " << i << ", " << j << " | new tour: ";
			for(auto num:slice1) cout << num << " ";
			cout << endl; 
		}
		cout << endl; 
	}
	return 0;
}