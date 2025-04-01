#include "View.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <Magick++.h>
#include <ImageCompressor.hpp>
#include <RunParams.hpp>
using namespace std;

RunParams runParams;
// Program entry point wrapper function
void run(){
    startView();
    paramsInput();
    compressImage(runParams);
}
void startView(){
    bool isValid = false;
    cout << "Quadtree Image Compressor" << endl;
    cout << "Enter Absolute File Path of the image that you would like to compress" << endl;    
    while (!isValid) {
        string imagePath;
        cin >> imagePath;
        if (filesystem::exists(imagePath)) {
            cout << "File exists" << endl;
            Magick::Image image(imagePath);
            runParams.image = image;
            runParams.fileSize = filesystem::file_size(imagePath);
            runParams.imageName = filesystem::path(imagePath).stem().string();
            runParams.imageWidth = image.columns();
            runParams.imageHeight = image.rows();
            isValid = true;
        } else {
            cout << "File does not exist" << endl;
            cout << "Please enter a valid file path :" << endl;
        }
    }
}
void paramsInput(){
    cout << "Enter the parameters for the quadtree compression" << endl;
    metricInput();
    thresholdInput();
    minBlockInput();
}
void metricInput(){
    bool isValid = false;
    cout << "Enter the Error Metric you would like to use :" << endl;
    cout<< "1. Variance" << endl;
    cout<< "2. Mean Absolute Deviation" << endl;
    cout<< "3. Max Pixel Difference" << endl;
    cout<< "4. Entropy" << endl;
    cout<< "5. Structural Similarity Index (SSIM)" << endl;
    int metric;
    while(!isValid) {
        cin >> metric;
        if (metric < 1 || metric > 5) {
            cout << "Invalid metric. Please enter a number between 1 and 5." << endl;
        } else {
            isValid = true;
            runParams.errorMetric = metric;
        }
    }
}
void thresholdInput(){
    bool isValid = false;
    cout << "Enter the threshold value for the quadtree compression" << endl;
    double threshold;
    while(!isValid) {
        cin >> threshold;
        if (threshold < 0) {
            cout << "Invalid threshold. Please enter a positive number" << endl;
        } else {
            isValid = true;
            runParams.threshold = threshold;
        }
    }
}
void minBlockInput(){
    bool isValid = false;
    cout << "Enter the minimum block size for the quadtree compression" << endl;
    int minBlock;
    while(!isValid) {
        cin >> minBlock;
        if (minBlock < 1) {
            cout << "Invalid minimum block size. Please enter a positive integer" << endl;
        } else {
            isValid = true;
            runParams.minBlock = minBlock;
        }
    }
}
void startCompression(){
    cout << "Starting compression..." << endl;
    
}
void saveImage(const Magick::Image& image, const string& path) {
    cout << "Image saved to " << path << endl;
}
void saveGif(const Magick::Image& image, const string& path) {
    cout << "GIF saved to " << path << endl;
}