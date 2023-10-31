#ifndef QUAD_TREE_NODE_H
#define QUAD_TREE_NODE_H

#include "Common.h"
#include "VerletObj.h"

class QuadTreeNode
{
public:
    QuadTreeNode(const int depth,
                 const int x, const int y,
                 const int width, const int height,
                 QuadTreeNode * const parent = nullptr) :
        depth_(depth),
        x_(x),
        y_(y),
        width_(width),
        height_(height),
        has_children_(false),
        parent_(parent),
        q1_(nullptr),
        q2_(nullptr),
        q3_(nullptr),
        q4_(nullptr)
    {}

    ~QuadTreeNode()
    {
        if (q1_ != nullptr)
            delete q1_;
        if (q2_ != nullptr)
            delete q2_;
        if (q3_ != nullptr)
            delete q3_;
        if (q4_ != nullptr)
            delete q4_;
    }

    bool has_children() const { return has_children_; }
    bool & has_children() { return has_children_; }

    void set_objects(const std::vector< VerletObj* > & objects)
    { objects_ = objects; }

    bool rects_intersect(const VerletObj & obj,
                         const int x, const int y,
                         const int w, const int h);

    void distribute_objects(const int threshold,
                            const int max_depth);

    void get_collisions(std::vector< std::pair< VerletObj*, VerletObj* > > & collisions) const;
    
private:
    const int depth_;
    const int x_;
    const int y_;
    const int width_;
    const int height_;
    
    bool has_children_;
    QuadTreeNode * parent_;
    // Quadrant 1, 2, 3, 4.
    QuadTreeNode * q1_, * q2_, * q3_, * q4_;
    
    std::vector< VerletObj* > objects_;
};

#endif
