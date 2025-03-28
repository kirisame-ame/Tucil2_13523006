#include <iostream>
#include <Magick++.h>
#include <ImageCompressor.hpp>
#include <Quadtree.hpp>
#include <Metrics.hpp>
using namespace std;
using namespace Magick;

int main(){
    cout<<"oi"<<endl;
    // Initialize ImageMagick
    InitializeMagick(nullptr);
    // Load the image
    Image image("C:/Users/jason/Documents/Code/Tucil2_13523006/assets/saiba.jpeg");
    // get rgb values of the image
    int width = image.columns();
    int height = image.rows();
    cout << "Width: " << width << ", Height: " << height << endl;
    RGB pixels;
    pixels[0].resize(width * height);
    pixels[1].resize(width * height);
    pixels[2].resize(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            ColorRGB color = image.pixelColor(x, y);
            pixels[0][y * width + x] = static_cast<int>(color.red() * 255);
            pixels[1][y * width + x] = static_cast<int>(color.green() * 255);
            pixels[2][y * width + x] = static_cast<int>(color.blue() * 255);
        }
    }
    // Calculate metrics
    double var = colorVariance(pixels);
    double mad = colorMeanAbsDev(pixels);
    int maxDiff = colorMaxPixelDiff(pixels);
    double ent = colorEntropy(pixels);
    cout << "Variance: " << var << endl;
    cout << "Mean Absolute Deviation: " << mad << endl;
    cout << "Max Pixel Difference: " << maxDiff << endl;
    cout << "Entropy: " << ent << endl;
    return 0;
}