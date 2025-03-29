#include "ImageCompressor.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <Magick++.h>
#include <Quadtree.hpp>
#include <Metrics.hpp>
using namespace std;

RGB getRGB(const Magick::Image& image) {
    int width = image.columns();
    int height = image.rows();
    RGB pixels;
    pixels[0].resize(width * height);
    pixels[1].resize(width * height);
    pixels[2].resize(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Magick::ColorRGB color = image.pixelColor(x, y);
            pixels[0][y * width + x] = (color.red()*255);
            pixels[1][y * width + x] = (color.green()*255);
            pixels[2][y * width + x] = (color.blue()*255);
        }
    }
    return pixels;
}