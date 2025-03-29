#ifndef IMAGECOMPRESSOR_HPP
#define IMAGECOMPRESSOR_HPP
#include <Magick++.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
using namespace std;
using RGB = array<vector<int>, 3>;

RGB getRGB(const Magick::Image& image);

#endif