#include "View.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <ImageCompressor.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>
#include <RunParams.hpp>

using namespace std;

RunParams runParams;
// Program entry point wrapper function
void run(){
    startView();
    paramsInput();
    startCompression();
}
void startView(){
    bool isValid = false;
    cout << "Quadtree Image Compressor" << endl<<endl;
    cout << "Enter Absolute File Path of the image that you would like to compress: " << endl;    
    while (!isValid) {
        string imagePath;
        cin >> imagePath;
        if (filesystem::exists(imagePath)) {
            cout << "File exists" << endl;
            runParams.fileSize = filesystem::file_size(imagePath);
            auto imgPix = stbi_load(imagePath.c_str(), &runParams.imageWidth, &runParams.imageHeight, NULL, 3);
            if (imgPix == nullptr) {
                cout << "Failed to load image" << endl;
                return;
            }
            runParams.imageBuffer = getRGB(imgPix, runParams.imageWidth, runParams.imageHeight);
            runParams.extension = filesystem::path(imagePath).extension().string();
            isValid = true;
            cout << "Image loaded successfully" << endl;
            cout << "Enter Absolute Directory Path to save the compressed image" << endl;
            cout <<"Or type 0 to save in the output directory: " << endl;
            string outputPath;
            cin >> outputPath;
            if (outputPath == "0") {
                cout << "Saving in output directory" << endl;
                runParams.outputPath = filesystem::current_path().parent_path().string()+"/output/"+filesystem::path(imagePath).stem().string()+"_compressed" + runParams.extension;
            } else {
                cout<<"Enter the file name to save the compressed image:" << endl;
                string fileName;
                cin>> fileName;
                runParams.outputPath = outputPath+"/"+fileName + runParams.extension;
            }
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
    cout<< "5. Structural Similarity Index (SSIM) NOTE: Minimum Threshold" << endl;
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
    cout<< "Auto or Manual Thresholding (y/n)" << endl;
    char autoThreshold;
    while(!isValid) {
        cin >> autoThreshold;
        if (autoThreshold == 'y' || autoThreshold == 'Y') {
            isValid = true;
        } else if (autoThreshold == 'n' || autoThreshold == 'N') {
            isValid = true;
        } else {
            cout << "Invalid input. Please enter y or n." << endl;
        }
    }
    isValid = false;
    if (autoThreshold == 'y' || autoThreshold == 'Y') {
        runParams.isAutoThreshold = 1;
        cout << "Auto thresholding selected" << endl;
        cout << "Enter the compression ratio (0-1) for the quadtree compression: " << endl;
        double compressionRatio;
        while(!isValid) {
            cin >> compressionRatio;
            if (compressionRatio <= 0 || compressionRatio >= 1) {
                cout << "Invalid compression ratio. Please enter a number between 0 and 1" << endl;
            } else {
                isValid = true;
                runParams.threshold = compressionRatio;
            }
        }
        cout << "Compression ratio set to " << compressionRatio * 100 << "%" << endl;
    } else {
        runParams.isAutoThreshold = 0;
        cout << "Manual thresholding selected" << endl;
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
    
}
void minBlockInput(){
    bool isValid = false;
    cout << "Enter the minimum block size for the quadtree compression" << endl;
    int minBlock;
    while(!isValid) {
        cin >> minBlock;
        if (minBlock < 1 || minBlock > runParams.imageWidth*runParams.imageHeight) {
            cout << "Invalid minimum block size. Please enter a positive integer within the image size" << endl;
        } else {
            isValid = true;
            runParams.minBlock = minBlock;
        }
    }
}
void startCompression(){
    cout << "Starting compression..." << endl;
    auto start = chrono::high_resolution_clock::now();

    if(runParams.isAutoThreshold == 1) {
        thresholdCompression(runParams,runParams.threshold);
    } else {
        compressImage(runParams);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Compression completed in " << duration << " milliseconds" << endl;
    cout << "Quadtree compression completed" << endl;
}