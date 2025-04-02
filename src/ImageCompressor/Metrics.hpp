#ifndef METRICS_HPP
#define METRICS_HPP
#include <cmath>
#include <vector>
#include <array>
#include <numeric>
#include <RunParams.hpp>
using namespace std;
using RGB = array<vector<int>, 3>;
// Mean Colors of a 3xN matrix of integers
// where N is the number of pixels and each row is a color channel r,g,b
array<int,3> meanColor(const RGB& values);
// Variance of a vector of integers
double variance(const vector<int>& values,double mean);
// Color Variance from 3xN matrix of integers
// where N is the number of pixels and each row is a color channel r,g,b
double colorVariance(const RGB& values,array<int,3> meanColors);
// Mean Absolute Deviation of a vector of integers
double meanAbsDev(const vector<int>& values,double mean);
// Mean Absolute Deviation of a 3xN matrix of integers
// where N is the number of pixels and each row is a color channel r,g,b
double colorMeanAbsDev(const RGB& values,array<int,3> meanColors);
// Max Pixel Difference of a vector of integers
int maxPixelDiff(const vector<int>& values);
// Max Pixel Difference of a 3xN matrix of integers
// where N is the number of pixels and each row is a color channel r,g,b
int colorMaxPixelDiff(const RGB& values);
// Entropy of a vector of integers
double entropy(const vector<int>& values);
// Entropy of a 3xN matrix of integers
// where N is the number of pixels and each row is a color channel r,g,b
double colorEntropy(const RGB& values);
bool passThreshold(const RunParams& runParams, const RGB& pixels, const array<int,3>& meanColors);
RGB getRGB(const unsigned char *imgBuf ,int width, int height);

#endif