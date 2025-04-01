#include "ImageCompressor.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <time.h>
#include <Magick++.h>
#include <Quadtree.hpp>
#include <Metrics.hpp>
#include <RunParams.hpp>
using namespace std;

// Main Image Compression Wrapper Function
void compressImage(const RunParams& runParams) {
    int width = runParams.imageWidth;
    int height = runParams.imageHeight;
    RGB imgPix = getRGB(runParams.image);
    array<int,3> meanColors = meanColor(imgPix);
    if (width*height > runParams.minBlock && passThreshold(runParams, imgPix,meanColors)) {
        // Create the root node of the quadtree
        unique_ptr<Quadtree> root = make_unique<Quadtree>(0, meanColors, array<int,2>{0,0}, array<int,2>{width,height});
        // Build the quadtree
        clock_t start = clock();
        root->buildQuadtree(runParams, 0, 0, width, height, 0,imgPix);
        clock_t end = clock();
        double timeTaken = double(end - start) / CLOCKS_PER_SEC;
        cout << "Quadtree built in " << timeTaken << " seconds." << endl;
        // Save the compressed image
        string outputPath = "C:/Users/jason/Documents/Code/Tucil2_13523006/tests/"+runParams.imageName+"_compressed.png";
        saveCompressedImage(root, outputPath, width, height);
    } else {
        cout << "Image can't be compressed more." << endl;
    }
    
}
void saveCompressedImage(const unique_ptr<Quadtree>& root, const string& path,int width, int height) {
    Magick::Image output(Magick::Geometry(width, height), "white");
    root->constructImage(output);
    output.write(path);
    cout << "Compressed image saved to " << path << endl;
}

