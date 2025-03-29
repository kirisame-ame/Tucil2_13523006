add_library(ImageCompressorLib 
    ImageCompressor/ImageCompressor.cpp 
    ImageCompressor/ImageCompressor.hpp 
    ImageCompressor/Metrics.cpp
    ImageCompressor/Metrics.hpp
    ImageCompressor/Quadtree.cpp
    ImageCompressor/Quadtree.hpp
    ImageCompressor/View.cpp
    ImageCompressor/View.hpp
    ImageCompressor/RunParams.hpp)
find_package(PkgConfig REQUIRED)
pkg_check_modules(IMAGEMAGICK REQUIRED Magick++)
add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=8 -DMAGICKCORE_HDRI_ENABLE=0)
target_include_directories(ImageCompressorLib PRIVATE ${IMAGEMAGICK_INCLUDE_DIRS})
target_include_directories(ImageCompressorLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ImageCompressor)
