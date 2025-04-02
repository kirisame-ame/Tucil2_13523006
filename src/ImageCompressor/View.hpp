#ifndef VIEW_HPP
#define VIEW_HPP
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <ImageCompressor.hpp>
#include <stb_image.h>
#include <stb_image_write.h>
#include <RunParams.hpp>
using namespace std;

void run();
void startView();
void paramsInput();
void metricInput();
void thresholdInput();
void minBlockInput();
void startCompression();

#endif