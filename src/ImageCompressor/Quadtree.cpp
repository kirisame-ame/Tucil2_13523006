#include "Quadtree.hpp"

Quadtree::Quadtree(int depth, const array<int,3>& color, 
    const array<int,2>& position, const array<int,2>& size) : depth(depth), color(color), 
    isLeaf(true),position(position),size(size) {}

Quadtree::~Quadtree() {}
int Quadtree::getDepth() {
    return depth;
}
const array<int,3>& Quadtree::getColor() {
    return color;
}
bool Quadtree::getIsLeaf() {
    return isLeaf;
}
void Quadtree::buildQuadtree(const RunParams& runParams,int x,int y,int width,int height,int depth,RGB& imgPix) {
    RGB pixels;
    pixels[0].resize(width*height);
    pixels[1].resize(width*height);
    pixels[2].resize(width*height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            pixels[0][i*width+j] = imgPix[0][(y+i)*runParams.imageWidth+x+j];
            pixels[1][i*width+j] = imgPix[1][(y+i)*runParams.imageWidth+x+j];
            pixels[2][i*width+j] = imgPix[2][(y+i)*runParams.imageWidth+x+j];
        }
    }
    array<int,3> meanColors = meanColor(pixels);
    this->color = meanColors;
    if (width * height <= runParams.minBlock) {
        return;
    }
    if (passThreshold(runParams, pixels,meanColors)) {
        this->isLeaf = false;
        this->tl = make_unique<Quadtree>(depth + 1, array<int,3>{0,0,0}, array<int,2>{x,y}, array<int,2>{width/2,height/2});
        this->tr = make_unique<Quadtree>(depth + 1, array<int,3>{0,0,0}, array<int,2>{x+width/2,y}, array<int,2>{width-width/2,height/2});
        this->bl = make_unique<Quadtree>(depth + 1, array<int,3>{0,0,0}, array<int,2>{x,y+height/2}, array<int,2>{width/2,height-height/2});
        this->br = make_unique<Quadtree>(depth + 1, array<int,3>{0,0,0}, array<int,2>{x+width/2,y+height/2}, array<int,2>{width-width/2, height-height/2});
        this->tl->buildQuadtree(runParams, x, y, width/2, height/2, depth + 1,imgPix);
        this->tr->buildQuadtree(runParams, x + width/2, y, width-width/2, height/2, depth + 1,imgPix);
        this->bl->buildQuadtree(runParams, x, y + height/2, width/2, height-height/2, depth + 1,imgPix);
        this->br->buildQuadtree(runParams, x + width/2, y + height/2, width-width/2, height-height/2, depth + 1,imgPix);
    } else {
        this->isLeaf = true;
        this->tl = nullptr;
        this->tr = nullptr;
        this->bl = nullptr;
        this->br = nullptr;
    }
}
void Quadtree::constructImage(unsigned char* output,int imgWidth, int *maxDepth,int *nodeCount) {
    if (maxDepth) {
        *maxDepth = max(*maxDepth, depth);
    }
    if (nodeCount) {
        (*nodeCount)++;
    }
    if (isLeaf) {
        int x = position[0];
        int y = position[1];
        for (int i = 0; i < size[1]; ++i) {
            for (int j = 0; j < size[0]; ++j) {
                output[((y + i) * imgWidth + (x + j)) * 3] = color[0];
                output[((y + i) * imgWidth + (x + j)) * 3 + 1] = color[1];
                output[((y + i) * imgWidth + (x + j)) * 3 + 2] = color[2];
            }
        }
    } else {
        if (tl) tl->constructImage(output,imgWidth, maxDepth,nodeCount);
        if (tr) tr->constructImage(output,imgWidth, maxDepth,nodeCount);
        if (bl) bl->constructImage(output,imgWidth, maxDepth,nodeCount);
        if (br) br->constructImage(output,imgWidth, maxDepth,nodeCount);
    }
}