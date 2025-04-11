#include "ImageCompressor.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <array>
#include <Quadtree.hpp>
#include <Metrics.hpp>
#include <RunParams.hpp>
#include <stb_image_write.h>
using namespace std;

// Main Image Compression Wrapper Function
void compressImage(const RunParams& runParams) {
    int width = runParams.imageWidth;
    int height = runParams.imageHeight;
    RGB imgPix = runParams.imageBuffer;
    array<int,3> meanColors = meanColor(imgPix);
    if (width*height > runParams.minBlock && passThreshold(runParams, imgPix,meanColors,0, width*height,runParams.threshold)) {
        // Create the root node of the quadtree
        unique_ptr<Quadtree> root = make_unique<Quadtree>(0, meanColors, array<int,2>{0,0}, array<int,2>{width,height});
        // Build the quadtree
        root->buildQuadtree(runParams, 0, 0, width, height, 0,imgPix,runParams.threshold);
        // Save the compressed image
        string outputPath = runParams.outputPath; 
        if (filesystem::exists(outputPath)) {
            cout << "File already exists. Overwriting..." << endl;
        }
        saveCompressedImage(root, outputPath, width, height,runParams.extension,runParams.fileSize);
    } else {
        cout << "Image can't be compressed more." << endl;
    }
    
}
void saveCompressedImage(const unique_ptr<Quadtree>& root, const string& path,int width, int height,string extension,double origSize) {
    unsigned char* output = new unsigned char[width * height * 3];
    origSize /= 1024.0; // Convert to KB
    int maxDepth=0;
    int nodeCount = 0;
    root->constructImage(output, width, &maxDepth, &nodeCount);
    if (extension == ".png") {
        stbi_write_png(path.c_str(), width, height, 3, output, width * 3);
    } else if (extension == ".jpg" || extension == ".jpeg") {
        // Adjust quality based on original size and dimensions
        // Small memory sized images or large dimensions 
        //tend to have bigger sizes on higher quality writes
        if (origSize < 100 || width*height >2000*2000) {
            stbi_write_jpg(path.c_str(), width, height, 3, output, 60);
        } else {
            stbi_write_jpg(path.c_str(), width, height, 3, output, 80);
        }
    } else if (extension == ".bmp") {
        stbi_write_bmp(path.c_str(), width, height, 3, output);
    } else {
        cout << "Unsupported file format" << endl;
    }
    double fileSize = filesystem::file_size(path) / 1024.0; // Convert to KB
    cout<<"Image dimensions: " << width << "x" << height << endl;
    cout<<"Original image size: " << origSize << " KB" << endl;
    cout << "Compressed image size: " << fileSize << " KB" << endl;
    cout<<"Compression Percentage: "<<(1-(fileSize/origSize))*100<<"%"<<endl;
    cout<<"Quadtree Depth: "<<maxDepth<<endl;
    cout<<"Node Count: "<<nodeCount<<endl;
    cout << "Compressed image saved to " << path << endl;
}
double saveTempImage(const unique_ptr<Quadtree>& root, const string& path,int width, int height,string extension,double origSize) {
    unsigned char* output = new unsigned char[width * height * 3];
    int maxDepth=0;
    int nodeCount = 0;
    root->constructImage(output, width, &maxDepth, &nodeCount);
    if (extension == ".png") {
        stbi_write_png(path.c_str(), width, height, 3, output, width * 3);
    } else if (extension == ".jpg" || extension == ".jpeg") {
        stbi_write_jpg(path.c_str(), width, height, 3, output, 80);
    } else if (extension == ".bmp") {
        stbi_write_bmp(path.c_str(), width, height, 3, output);
    } else {
        cout << "Unsupported file format" << endl;
    }

    double fileSize = filesystem::file_size(path) / 1024.0; // Convert to KB
    origSize /= 1024.0; // Convert to KB
    double compRatio = (1-(fileSize/origSize));
    return compRatio;
}
void thresholdCompression(const RunParams& runParams,double compRatio) {
    double low = 0.0;
    double high = 0.0;
    if (runParams.errorMetric==1){
        low = 0.0;
        high = 10000.0;
    }else if (runParams.errorMetric==2){
        low = 0.0;
        high = 255.0;
    }
    else if (runParams.errorMetric==3){
        low = 0.0;
        high = 255.0;
    }
    else if (runParams.errorMetric==4){
        low = 0.0;
        high = 8.0;
    }
    else if (runParams.errorMetric==5){
        low = 0.0;
        high = 1.0;
    }
    else{
        cout<<"Invalid error metric"<<endl;
        return;
    }
    
    double bestThreshold = low;
    double tolerance = 0.01;

    int width = runParams.imageWidth;
    int height = runParams.imageHeight;
    RGB imgPix = runParams.imageBuffer;

    string tempOutput = runParams.outputPath + ".tmp" + runParams.extension;

    unique_ptr<Quadtree> root;

    for (int i = 0; i < 12; ++i) { // log2(4096) ~= 12 steps max
        double mid = (low + high) / 2.0;
        double threshold = mid;

        array<int,3> meanColors = meanColor(imgPix);

        root = make_unique<Quadtree>(0, meanColors, array<int,2>{0,0}, array<int,2>{width,height});
        root->buildQuadtree(runParams, 0, 0, width, height, 0, imgPix,threshold);

        double tempRatio = saveTempImage(root, tempOutput, width, height, runParams.extension, runParams.fileSize);
        cout << "[Threshold " << mid << "] Compression Ratio: " << tempRatio << endl;

        if (abs(tempRatio - compRatio) <= tolerance) {
            bestThreshold = mid;
            break;
        }
        if (tempRatio < compRatio) {
            cout<<"Compression Ratio too low, increasing threshold..."<<endl;  
            low = mid;
        } else {
            cout<<"Compression Ratio too high, decreasing threshold..."<<endl;
            high = mid;
        }
        bestThreshold = mid;
    }   
    cout << "Best threshold: " << bestThreshold << endl;
    filesystem::remove(tempOutput); // Remove temp file
    saveCompressedImage(root, runParams.outputPath, width, height, runParams.extension, runParams.fileSize);
}
