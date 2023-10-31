#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "Common.h"
#include "VerletObj.h"
#include "QuadTreeNode.h"

class QuadTree
{
public:
    QuadTree(const int x, const int y,
             const int width, const int height,
             const int threshold, const int max_depth,
             std::vector< VerletObj* > * const objects) :
        x_(x),
        y_(y),
        width_(width),
        height_(height),
        threshold_(threshold),
        max_depth_(max_depth),
        objects_(objects),
        root_(nullptr)
    {}

    ~QuadTree()
    {
        if (root_ != nullptr)
            delete root_;
    }

    void update();

    std::vector< std::pair< VerletObj*, VerletObj* > > get_collisions() const;
        
private:
    const int x_;
    const int y_;
    const int width_;
    const int height_;
    const int threshold_;
    const int max_depth_;
    
    std::vector< VerletObj* > * objects_;
    
    QuadTreeNode* root_;
};

#endif
