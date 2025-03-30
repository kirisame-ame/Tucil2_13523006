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
void Quadtree::buildQuadtree(const RunParams& runParams,int x,int y,int width,int height,int depth) {
    cout<<"Building quadtree at depth " << depth << " with size " << width << "x" << height << endl;
    cout<<"Position: (" << x << "," << y << ")" << endl;
    if (width * height <= runParams.minBlock) {
        return;
    }
    // Create a new image block from the original image
    Magick::Image block(runParams.image);
    block.crop(Magick::Geometry(width, height, x, y));
    RGB pixels = getRGB(block);
    array<int,3> meanColors = meanColor(pixels);
    cout<<"Mean Colors: (" << meanColors[0] << "," << meanColors[1] << "," << meanColors[2] << ")" << endl;
    this->color = meanColors;
    if (passThreshold(runParams, pixels,meanColors)) {
        this->isLeaf = false;
        this->tl = make_unique<Quadtree>(depth + 1, array<int,3>{0,0,0}, array<int,2>{x,y}, array<int,2>{width/2,height/2});
        this->tr = make_unique<Quadtree>(depth + 1, array<int,3>{0,0,0}, array<int,2>{x+width/2,y}, array<int,2>{width/2,height/2});
        this->bl = make_unique<Quadtree>(depth + 1, array<int,3>{0,0,0}, array<int,2>{x,y+height/2}, array<int,2>{width/2,height/2});
        this->br = make_unique<Quadtree>(depth + 1, array<int,3>{0,0,0}, array<int,2>{x+width/2,y+height/2}, array<int,2>{width/2,height/2});
        this->tl->buildQuadtree(runParams, x, y, width/2, height/2, depth + 1);
        this->tr->buildQuadtree(runParams, x + width/2, y, width/2, height/2, depth + 1);
        this->bl->buildQuadtree(runParams, x, y + height/2, width/2, height/2, depth + 1);
        this->br->buildQuadtree(runParams, x + width/2, y + height/2, width/2, height/2, depth + 1);
    } else {
        this->isLeaf = true;
        this->tl = nullptr;
        this->tr = nullptr;
        this->bl = nullptr;
        this->br = nullptr;
    }
}
void Quadtree::constructImage(Magick::Image &output) {
    if (isLeaf) {
        // Fill the block with the color of this node
        cout<<"Filling block at position (" << position[0] << "," << position[1] << ") with color (" << color[0] << "," << color[1] << "," << color[2] << ")" << endl;
        output.fillColor(Magick::ColorRGB(color[0]/255, color[1]/255, color[2]/255));
        output.draw(Magick::DrawableRectangle(position[0], position[1], position[0] + size[0], position[1] + size[1]));
    } else {
        if (tl) tl->constructImage(output);
        if (tr) tr->constructImage(output);
        if (bl) bl->constructImage(output);
        if (br) br->constructImage(output);
    }
}