#include "Metrics.hpp"
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;
using RGB = array<vector<int>, 3>;

array<int,3> meanColor(const RGB& values){
    vector<int> r = values[0];
    vector<int> g = values[1];
    vector<int> b = values[2];
    int size = r.size();
    int meanR = 0;
    int meanG = 0;
    int meanB = 0;
    for(int i = 0; i < size; i++){
        meanR += r[i];
        meanG += g[i];
        meanB += b[i];
    }
    meanR /= size;
    meanG /= size;
    meanB /= size;
    return {meanR,meanG,meanB};
}
double variance(const vector<int>& values){
    int size = values.size();
    double mean = 0;
    for(int i = 0; i < size; i++){
        mean += values[i];
    }
    mean /= size;
    double variance = 0;
    for(int i = 0; i < size; i++){
        variance += pow(values[i] - mean, 2);
    }
    variance /= size;
    return variance;
}
double colorVariance(const RGB& values){
    vector<int> r = values[0];
    vector<int> g = values[1];
    vector<int> b = values[2];
    return (variance(r) + variance(g) + variance(b))/3;
}
double meanAbsDev(const vector<int>& values){
    int size = values.size();
    double mean = 0;
    for(int i = 0; i < size; i++){
        mean += values[i];
    }
    mean /= size;
    double mad = 0;
    for(int i = 0; i < size; i++){
        mad += abs(values[i] - mean);
    }
    mad /= size;
    return mad;
}
double colorMeanAbsDev(const RGB& values){
    vector<int> r = values[0];
    vector<int> g = values[1];
    vector<int> b = values[2];
    return (meanAbsDev(r) + meanAbsDev(g) + meanAbsDev(b))/3;
}
int maxPixelDiff(const vector<int>& values){
    int max = 0;
    int min = 256;
    int size = values.size();
    for(int i = 0; i < size; i++){
        if(values[i] > max){
            max = values[i];
        }
        if(values[i] < min){
            min = values[i];
        }
    }
    return max-min;
}
int colorMaxPixelDiff(const RGB& values){
    vector<int> r = values[0];
    vector<int> g = values[1];
    vector<int> b = values[2];
    return (maxPixelDiff(r)+ maxPixelDiff(g)+ maxPixelDiff(b))/3;
}
double entropy(const vector<int>& values){
    int size = values.size();
    vector<int> freq(256, 0);
    for(int i = 0; i < size; i++){
        freq[values[i]]++;
    }
    double entropy = 0;
    for(int i = 0; i < 256; i++){
        if(freq[i] == 0){
            continue;
        }
        double p = (double)freq[i]/size;
        entropy -= p*log2(p);
    }
    return entropy;
}
double colorEntropy(const RGB& values){
    vector<int> r = values[0];
    vector<int> g = values[1];
    vector<int> b = values[2];
    return (entropy(r) + entropy(g) + entropy(b))/3;
}