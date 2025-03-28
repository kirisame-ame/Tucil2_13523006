#ifndef QUADTREE_HPP
#define QUADTREE_HPP
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Quadtree
{
private:
    unique_ptr<Quadtree> tl;
    unique_ptr<Quadtree> tr;
    unique_ptr<Quadtree> bl;
    unique_ptr<Quadtree> br;
    int depth;
    vector<int> position;
    vector<int> size;
    vector<int> color;
    bool isLeaf;
public:
    // Leaf constructor
    Quadtree(int depth, const vector<int>& color, const vector<int>& position, 
        const vector<int>& size);
    // Node constructor
    Quadtree(int depth, unique_ptr<Quadtree> tl, unique_ptr<Quadtree> tr, 
        unique_ptr<Quadtree> bl, unique_ptr<Quadtree> br, 
        const vector<int>& position, const vector<int>& size);
    ~Quadtree();
    // Get depth of current node
    int getDepth();
    // Get color of current node
    const vector<int>& getColor();
    // Get isLeaf of current node
    bool getIsLeaf();
};
#endif