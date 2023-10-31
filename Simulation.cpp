#include "Simulation.h"


sf::RenderWindow * Simulation::w_ptr_ = nullptr;


void Simulation::update_objects(const float dt)
{
    float sub_dt = dt / float(sub_steps_);
    for (int i = 0; i < sub_steps_; ++i)
    {
        kill_out_of_bounds();
        apply_gravity();
        apply_constraint();
        adjust_for_collisions();
        move_objects(sub_dt);
    }

    return;
}

void Simulation::kill_out_of_bounds()
{
    for (int i = 0; i < objects_.size(); ++i)
    {
        int x = objects_[i]->getPosition().x;
        int y = objects_[i]->getPosition().y;
        int r = objects_[i]->getRadius();
        if (x + r < 0 || x - r > WIDTH || y + r < 0 || y - r > HEIGHT)
        {
            delete objects_[i];
            objects_.erase(objects_.begin() + i);
            --i;
        }
    }

    return;
}



void Simulation::move_objects(const float dt)
{
    for (VerletObj * & v_o : objects_)
        v_o->update(dt);

    return;
}


void Simulation::apply_gravity()
{
    for (VerletObj * & v_o : objects_)
        v_o->accelerate(gravity_);

    return;
}


void Simulation::apply_constraint()
{
    for (VerletObj * & v_o : objects_)
    {
        /*
          std::cout << '\n';
          std::cout << "bound_pos: " << bounds_.getPosition() << std::endl;
          std::cout << "obj_pos: " << v_o.getPosition() << std::endl;
        */
        sf::Vector2f to_obj = bounds_.getPosition() - v_o->getPosition();
        //std::cout << "to_obj: " << to_obj << std::endl;
        float len = sqrt(to_obj.x * to_obj.x + to_obj.y * to_obj.y);

        if (len + v_o->getRadius() > bounds_.getRadius())
        {
            sf::Vector2f div = to_obj / len;
            //std::cout << "len: " << len << std::endl;
            //std::cout << "div: " << div << std::endl;
            sf::Vector2f move_vec = div * (bounds_.getRadius() - v_o->getRadius());
            //std::cout << "move_vec: " << move_vec << std::endl;
            v_o->setPosition(bounds_.getPosition() - move_vec);
        }
    }

    return;
}


void Simulation::adjust_for_collisions()
{
    // Quad Tree is bugging out.
    if (true)
    {
        std::vector< std::pair< VerletObj*, VerletObj* > > collisions;
        quad_tree_->update();

        collisions = quad_tree_->get_collisions();

        // This map will make sure not to compute repeats.
        std::unordered_map< VerletObj*, std::unordered_set< VerletObj* > > map;
    
        int n = collisions.size();
        for (int i = 0; i < n; ++i)
        {
            if (map.find(collisions[i].first) == map.end() ||
                map[collisions[i].first].find(collisions[i].second) == map[collisions[i].first].end())
            {
                VerletObj *obj_0 = collisions[i].first, *obj_1 = collisions[i].second;
                //std::cout << "COLLISION!!!";
                sf::Vector2f to_obj = obj_0->getPosition() - obj_1->getPosition();
                float len = sqrt(to_obj.x * to_obj.x + to_obj.y * to_obj.y);
                //std::cout << ' ' << len << std::endl;
                sf::Vector2f div = to_obj / len;
                float delta = obj_0->getRadius() + obj_1->getRadius() - len;
                sf::Vector2f move_vec = div * delta;
                obj_0->move(move_vec / 2.f);
                obj_1->move(move_vec / -2.f);

                // Store the first object in the set that corresponds
                // to the second object so it does not compute the same
                // collision twice.
                map[obj_1].insert(obj_0);
                map[obj_0].insert(obj_1);
            }
        }
    }
    
    else
    {
        int n = objects_.size();
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                if (objects_[i]->collides(*objects_[j]))
                {
                    //std::cout << "COLLISION!!!";
                    sf::Vector2f to_obj = objects_[i]->getPosition() - objects_[j]->getPosition();
                    float len = sqrt(to_obj.x * to_obj.x + to_obj.y * to_obj.y);
                    //std::cout << ' ' << len << std::endl;
                    sf::Vector2f div = to_obj / len;
                    float delta = objects_[i]->getRadius() + objects_[j]->getRadius() - len;
                    sf::Vector2f move_vec = div * delta;
                    objects_[i]->move(move_vec / 2.f);
                    objects_[j]->move(move_vec / -2.f);
                }
            }
        }
    }

    return;
}


void Simulation::draw() const
{
    if (w_ptr_ == nullptr)
        throw Window_Not_Set_Error();

    w_ptr_->draw(bounds_);
        
    for (VerletObj * v_o : objects_)
        w_ptr_->draw(*v_o);

    return;
}


void Simulation::create_obj()
{
    create_obj_spray();
    
    return;
}

void Simulation::create_obj_spray()
{
    static int created = 0;
    static int step = 0;
    double mult = M_PI / 80.0;

    double vel_x = 100000 * sin(created * mult); //???
    double vel_y = 100000;
    
    if (created < 1000)
    {
        VerletObj * ver = nullptr;
        if (!objects_.empty())
            ver = objects_.back();
        
        int r = 5; //created % 40 + 5;
        sf::Color c = sf::Color::Red;
        if (ver != nullptr)
            c = ver->getFillColor();
        c = get_color(step, r, c);
        
        objects_.push_back(new VerletObj(r, WIDTH / 2, 50, c));
        
        int n = objects_.size();
        bool valid = (ver == nullptr || !objects_.back()->collides(*ver));

        if (valid)
        {
            objects_.back()->accelerate(sf::Vector2f(vel_x, vel_y));
            ++created;
        }
        else
        {
            delete objects_.back();
            objects_.pop_back();
        }
    }

}
