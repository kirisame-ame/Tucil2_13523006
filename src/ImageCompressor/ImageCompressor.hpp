#ifndef IMAGECOMPRESSOR_HPP
#define IMAGECOMPRESSOR_HPP
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <array>
#include <filesystem>
#include <fstream>
#include <Metrics.hpp>
#include <Quadtree.hpp>
#include <RunParams.hpp>
#include <stb_image_write.h>
using namespace std;
using RGB = array<vector<int>, 3>;

void compressImage(const RunParams& runParams);
void saveCompressedImage(const unique_ptr<Quadtree>& root, const string& path,int width, int height,string extension,double origSize);

#endif