#ifndef IMAGECOMPRESSOR_HPP
#define IMAGECOMPRESSOR_HPP
#include <Magick++.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <array>
#include <Metrics.hpp>
#include <Quadtree.hpp>
#include <RunParams.hpp>
using namespace std;
using RGB = array<vector<int>, 3>;

void compressImage(const RunParams& runParams);
void saveSnapshot(const Magick::Image& image, const string& path);
void saveCompressedImage(const unique_ptr<Quadtree>& root, const string& path,int width, int height);

#endif