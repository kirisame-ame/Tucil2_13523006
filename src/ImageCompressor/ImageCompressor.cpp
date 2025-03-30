#include "ImageCompressor.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <Magick++.h>
#include <Quadtree.hpp>
#include <Metrics.hpp>
#include <RunParams.hpp>
using namespace std;

// Main Image Compression Wrapper Function
void compressImage(const RunParams& runParams) {
    int width = runParams.image.columns();
    int height = runParams.image.rows();
    array<int,3> meanColors = meanColor(getRGB(runParams.image));
    if (width*height > runParams.minBlock && passThreshold(runParams, getRGB(runParams.image),meanColors)) {
        // Create the root node of the quadtree
        unique_ptr<Quadtree> root = make_unique<Quadtree>(0, meanColors, array<int,2>{0,0}, array<int,2>{width,height});
        // Build the quadtree
        root->buildQuadtree(runParams, 0, 0, width, height, 0);
        // Save the compressed image
        string outputPath = "C:/Users/jason/Documents/Code/Tucil2_13523006/tests/output.png";
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

