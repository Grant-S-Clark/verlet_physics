#include "QuadTree.h"

void QuadTree::update()
{
    if (root_ != nullptr)
        delete root_;

    root_ = new QuadTreeNode(0, x_, y_,
                             width_, height_);

    root_->set_objects(*objects_);
    root_->distribute_objects(threshold_,
                              max_depth_);
    
    return;
}

std::vector< std::pair< VerletObj*, VerletObj* > > QuadTree::get_collisions() const
{
    std::vector< std::pair< VerletObj*, VerletObj* > > collisions;

    root_->get_collisions(collisions);

    return collisions;
}
