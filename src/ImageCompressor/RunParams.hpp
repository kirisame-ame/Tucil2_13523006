#ifndef RUNPARAMS_HPP
#define RUNPARAMS_HPP
#include <string>
struct RunParams
{
    unsigned char* imageBuffer;
    double threshold;
    int fileSize;
    int imageWidth;
    int imageHeight;
    int minBlock;
    int errorMetric;
    std::string outputPath;
    std::string extension;
};
#endif