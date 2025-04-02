add_library(ImageCompressorLib 
    ImageCompressor/ImageCompressor.cpp 
    ImageCompressor/ImageCompressor.hpp 
    ImageCompressor/Metrics.cpp
    ImageCompressor/Metrics.hpp
    ImageCompressor/Quadtree.cpp
    ImageCompressor/Quadtree.hpp
    ImageCompressor/View.cpp
    ImageCompressor/View.hpp
    ImageCompressor/RunParams.hpp
    ImageCompressor/stb_image.h
    ImageCompressor/stb_image_write.h)
target_include_directories(ImageCompressorLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ImageCompressor)
