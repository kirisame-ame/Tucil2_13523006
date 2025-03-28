#include "Quadtree.hpp"

Quadtree::Quadtree(int depth, const vector<int>& color, 
    const vector<int>& position, const vector<int>& size) : depth(depth), color(color), 
    isLeaf(true),position(position),size(size) {}
Quadtree::Quadtree(int depth, unique_ptr<Quadtree> tl, unique_ptr<Quadtree> tr, 
                   unique_ptr<Quadtree> bl, unique_ptr<Quadtree> br, 
                   const vector<int>& position, const vector<int>& size) 
    : depth(depth), tl(move(tl)), tr(move(tr)), bl(move(bl)), br(move(br)), 
    isLeaf(false),position(position),size(size) {}
Quadtree::~Quadtree() {}
int Quadtree::getDepth() {
    return depth;
}
const vector<int>& Quadtree::getColor() {
    return color;
}
bool Quadtree::getIsLeaf() {
    return isLeaf;
}