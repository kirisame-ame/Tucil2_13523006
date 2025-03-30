#ifndef VIEW_HPP
#define VIEW_HPP
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <Magick++.h>
#include <ImageCompressor.hpp>
#include <RunParams.hpp>
using namespace std;

void run();
void startView();
void paramsInput();
void metricInput();
void thresholdInput();
void minBlockInput();
void startCompression();
void saveImage(const Magick::Image& image, const string& path);
void saveGif(const Magick::Image& image, const string& path);

#endif