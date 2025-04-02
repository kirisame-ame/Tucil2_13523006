#include "ImageCompressor.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <array>
#include <chrono>
#include <Quadtree.hpp>
#include <Metrics.hpp>
#include <RunParams.hpp>
#include <stb_image_write.h>
using namespace std;

// Main Image Compression Wrapper Function
void compressImage(const RunParams& runParams) {
    int width = runParams.imageWidth;
    int height = runParams.imageHeight;
    RGB imgPix = getRGB(runParams.imageBuffer, width, height);
    array<int,3> meanColors = meanColor(imgPix);
    if (width*height > runParams.minBlock && passThreshold(runParams, imgPix,meanColors)) {
        // Create the root node of the quadtree
        unique_ptr<Quadtree> root = make_unique<Quadtree>(0, meanColors, array<int,2>{0,0}, array<int,2>{width,height});
        // Build the quadtree
        auto start = chrono::high_resolution_clock::now();
        root->buildQuadtree(runParams, 0, 0, width, height, 0,imgPix);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Quadtree built in " << duration << " ms" << endl;
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
    int maxDepth;
    int nodeCount = 0;
    root->constructImage(output, width, &maxDepth, &nodeCount);
    if (extension == ".png") {
        stbi_write_png(path.c_str(), width, height, 3, output, width * 3);
    } else if (extension == ".jpg" || extension == ".jpeg") {
        stbi_write_jpg(path.c_str(), width, height, 3, output, 75);
    } else if (extension == ".bmp") {
        stbi_write_bmp(path.c_str(), width, height, 3, output);
    } else {
        cout << "Unsupported file format" << endl;
    }
    double fileSize = filesystem::file_size(path) / 1024.0; // Convert to KB
    origSize /= 1024.0; // Convert to KB
    cout<<"Original image size: " << origSize << " KB" << endl;
    cout << "Compressed image size: " << fileSize << " KB" << endl;
    cout<<"Compression Percentage: "<<(1-(fileSize/origSize))*100<<"%"<<endl;
    cout<<"Quadtree Depth: "<<maxDepth<<endl;
    cout<<"Node Count: "<<nodeCount<<endl;
    cout << "Compressed image saved to " << path << endl;
}

