#ifndef RUNPARAMS_HPP
#define RUNPARAMS_HPP
#include <Magick++.h>
struct RunParams
{
    Magick::Image image;
    double threshold;
    int minBlock;
    int errorMetric;
};
#endif