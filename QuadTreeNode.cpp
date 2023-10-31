#include "QuadTreeNode.h"

bool QuadTreeNode::rects_intersect(const VerletObj & obj,
                                   const int x, const int y,
                                   const int w, const int h)
{
    sf::FloatRect rect(x, y, w, h);
    return obj.getGlobalBounds().intersects(rect);
}

void QuadTreeNode::distribute_objects(const int threshold,
                                      const int max_depth)
{
    // no need to distribute.
    if (objects_.size() <= threshold ||
        depth_ >= max_depth)
    {
        return;
    }

    has_children_ = true;

    q1_ = new QuadTreeNode(depth_ + 1,
                           x_ + width_ / 2, y_,
                           width_ / 2, height_ / 2,
                           this);
    q2_ = new QuadTreeNode(depth_ + 1,
                           x_, y_,
                           width_ / 2, height_ / 2,
                           this);
    q3_ = new QuadTreeNode(depth_ + 1,
                           x_, y_ + height_ / 2,
                           width_ / 2, height_ / 2,
                           this);
    q4_ = new QuadTreeNode(depth_ + 1,
                           x_ + width_ / 2, y_ + height_ / 2,
                           width_ / 2, height_ / 2,
                           this);

    std::vector< std::vector< VerletObj* > > distributions(4);

    short quadrant;
    for (int i = 0; i < objects_.size(); ++i)
    {
        quadrant = 0;

        if (rects_intersect(*(objects_[i]),
                            q1_->x_, q1_->y_,
                            q1_->width_, q1_->height_))
        {
            quadrant = 1;
        }

        if (rects_intersect(*(objects_[i]),
                            q2_->x_, q2_->y_,
                            q2_->width_, q2_->height_))
        {
            if (quadrant != 0)
                continue;
            quadrant = 2;
        }

        if (rects_intersect(*(objects_[i]),
                            q3_->x_, q3_->y_,
                            q3_->width_, q3_->height_))
        {
            if (quadrant != 0)
                continue;
            quadrant = 3;
        }

        if (rects_intersect(*(objects_[i]),
                            q4_->x_, q4_->y_,
                            q4_->width_, q4_->height_))
        {
            if (quadrant != 0)
                continue;
            quadrant = 4;
        }

        if (quadrant != 0)
        {
            distributions[quadrant - 1].push_back(objects_[i]);
            objects_.erase(objects_.begin() + i);
            --i;
        }
    }

    // Give distributions to children.
    q1_->set_objects(distributions[0]);
    q2_->set_objects(distributions[1]);
    q3_->set_objects(distributions[2]);
    q4_->set_objects(distributions[3]);

    // Distribute through children.
    q1_->distribute_objects(threshold, max_depth);
    q2_->distribute_objects(threshold, max_depth);
    q3_->distribute_objects(threshold, max_depth);
    q4_->distribute_objects(threshold, max_depth);
    
    return;
}

void QuadTreeNode::get_collisions(std::vector< std::pair< VerletObj*, VerletObj* > > & collisions) const
{
    // Check all objects on this leaf.
    for (int i = 0, n = objects_.size(); i < n - 1; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (objects_[i]->collides(*(objects_[j])))
            {
                collisions.push_back({objects_[i], objects_[j]});
            }
        }
    }

    // Check all objects on all parent leaves.
    QuadTreeNode * p = parent_;
    while (p != nullptr)
    {
        for (int i = 0, n = objects_.size(); i < n; ++i)
        {
            for (int j = 0, m = p->objects_.size(); j < m; ++j)
            {
                if (objects_[i]->collides(*(p->objects_[j])))
                {
                    collisions.push_back({objects_[i], p->objects_[j]});
                }
            }
        }
        
        p = p->parent_;
    }

    // Recurse down through children.
    if (has_children_)
    {
        q1_->get_collisions(collisions);
        q2_->get_collisions(collisions);
        q3_->get_collisions(collisions);
        q4_->get_collisions(collisions);
    }

    return;
}
