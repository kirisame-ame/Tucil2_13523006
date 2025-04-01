#ifndef RUNPARAMS_HPP
#define RUNPARAMS_HPP
#include <Magick++.h>
struct RunParams
{
    Magick::Image image;
    double threshold;
    int fileSize;
    int imageWidth;
    int imageHeight;
    int minBlock;
    int errorMetric;
    std::string imageName;
};
#endif