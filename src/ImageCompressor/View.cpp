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
void run(){
    startView();
    paramsInput();
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
            isValid = true;
        } else {
            cout << "File does not exist" << endl;
            cout << "Please enter a valid file path :" << endl;
        }
    }
}
void paramsInput(){
    cout << "Enter the parameters for the quadtree compression" << endl;
    cout << "Enter the Error Metric you would like to use :" << endl;
    int maxDepth;
    cin >> maxDepth;
    cout << "Enter the split threshold :" << endl;
    int minSize;
    cin >> minSize;
    cout << "Enter the minimum block size for a split :" << endl;
    int maxSize;
    cin >> maxSize;
}
void saveImage(const Magick::Image& image, const string& path) {
    cout << "Image saved to " << path << endl;
}