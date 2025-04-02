#ifndef QUADTREE_HPP
#define QUADTREE_HPP
#include <iostream>
#include <memory>
#include <array>
#include <vector>
#include <RunParams.hpp>
#include <Metrics.hpp>
using namespace std;

class Quadtree
{
private:
    unique_ptr<Quadtree> tl;
    unique_ptr<Quadtree> tr;
    unique_ptr<Quadtree> bl;
    unique_ptr<Quadtree> br;
    int depth;
    array<int,2> position;
    array<int,2> size;
    array<int,3> color;
    bool isLeaf;
public:
    //constructor
    Quadtree(int depth, const array<int,3>& color, const array<int,2>& position, 
        const array<int,2>& size);
    ~Quadtree();
    // Get depth of current node
    int getDepth();
    // Get color of current node
    const array<int,3>& getColor();
    // Get isLeaf of current node
    bool getIsLeaf();
    void buildQuadtree(const RunParams& runParams,int x,int y,int width,int height,int depth,RGB& imgPix);
    void constructImage(unsigned char *output,int imgWidth, int *maxDepth,int *nodeCount);
};
#endif