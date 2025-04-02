#include "Metrics.hpp"
#include <cmath>
#include <vector>
#include <iostream>
#include <RunParams.hpp>
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
double variance(const vector<int>& values,double mean){
    int size = values.size();
    double variance = 0;
    for(int i = 0; i < size; i++){
        variance += pow(values[i] - mean, 2);
    }
    variance /= size;
    return variance;
}
double colorVariance(const RGB& values,array<int,3> meanColors){
    vector<int> r = values[0];
    vector<int> g = values[1];
    vector<int> b = values[2];
    return (variance(r,meanColors[0]) + variance(g,meanColors[1]) + variance(b,meanColors[2]))/3;
}
double meanAbsDev(const vector<int>& values,double mean){
    int size = values.size();
    double mad = 0;
    for(int i = 0; i < size; i++){
        mad += abs(values[i] - mean);
    }
    mad /= size;
    return mad;
}
double colorMeanAbsDev(const RGB& values,array<int,3> meanColors){
    vector<int> r = values[0];
    vector<int> g = values[1];
    vector<int> b = values[2];
    return (meanAbsDev(r,meanColors[0]) + meanAbsDev(g,meanColors[1]) + meanAbsDev(b,meanColors[2]))/3;
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
double singleSsim(const vector<int>& origImg, double mean1, double mean2) {
    double c1 = 6.5025, c2 = 58.5225;
    double origImgVar = variance(origImg, mean1);
    // Covariance is zero since newImg's pixels are all the same
    // Variance of newImg is zero since newImg's pixels are all the same
    return (2 * mean1 * mean2 + c1) * c2 / ((mean1 * mean1 + mean2 * mean2 + c1) * (origImgVar + c2));
}
double ssim(const RGB& origImg,const array<int,3>& meanColors,int index,int size) {
    double c1 = 6.5025, c2 = 58.5225;
    
    double sumR = 0, sumG = 0, sumB = 0;
    double sumSqR = 0, sumSqG = 0, sumSqB = 0;

    for (int i = index; i < index + size; ++i) {
        sumR += origImg[0][i]; sumSqR += origImg[0][i] * origImg[0][i];
        sumG += origImg[1][i]; sumSqG += origImg[1][i] * origImg[1][i];
        sumB += origImg[2][i]; sumSqB += origImg[2][i] * origImg[2][i];
    }

    double meanR = sumR / size, meanG = sumG / size, meanB = sumB / size;
    double varR = (sumSqR - (sumR * sumR) / size) / size;
    double varG = (sumSqG - (sumG * sumG) / size) / size;
    double varB = (sumSqB - (sumB * sumB) / size) / size;

    double redSSIM   = (2 * meanR * meanColors[0] + c1) * c2 / ((meanR * meanR + meanColors[0] * meanColors[0] + c1) * (varR + c2));
    double greenSSIM = (2 * meanG * meanColors[1] + c1) * c2 / ((meanG * meanG + meanColors[1] * meanColors[1] + c1) * (varG + c2));
    double blueSSIM  = (2 * meanB * meanColors[2] + c1) * c2 / ((meanB * meanB + meanColors[2] * meanColors[2] + c1) * (varB + c2));

    return (redSSIM + greenSSIM + blueSSIM) / 3.0;
}
bool passThreshold(const RunParams& runParams, const RGB& pixels, const array<int,3>& meanColors, int index, int size) {
    double error;
    switch (runParams.errorMetric) {
        case 1:
            error = colorVariance(pixels,meanColors);
            break;
        case 2:
            error = colorMeanAbsDev(pixels,meanColors);
            break;
        case 3:
            error = colorMaxPixelDiff(pixels);
            break;
        case 4:
            error = colorEntropy(pixels);
            break;
        case 5:
            error = 1-ssim(runParams.imageBuffer, meanColors,index, size);
            break;
        default:
            cout << "Invalid Error Metric" << endl;
            return false;
    }
    return error > runParams.threshold;
}
RGB getRGB(const unsigned char* imgBuf,int width, int height) {
    RGB pixels;
    pixels[0].resize(width * height);
    pixels[1].resize(width * height);
    pixels[2].resize(width * height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            pixels[0][i * width + j] = imgBuf[(i * width + j) * 3];
            pixels[1][i * width + j] = imgBuf[(i * width + j) * 3 + 1];
            pixels[2][i * width + j] = imgBuf[(i * width + j) * 3 + 2];
        }
    }
    return pixels;
}