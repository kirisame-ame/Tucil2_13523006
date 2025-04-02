#ifndef RUNPARAMS_HPP
#define RUNPARAMS_HPP
#include <string>
using RGB = std::array<std::vector<int>, 3>;
struct RunParams
{
    double threshold;
    int fileSize;
    int imageWidth;
    int imageHeight;
    int minBlock;
    int errorMetric;
    std::string outputPath;
    std::string extension;
    RGB imageBuffer;
};
#endif