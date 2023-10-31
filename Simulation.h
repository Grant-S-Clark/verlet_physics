#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <unordered_map>
#include <unordered_set>
#include "Common.h"
#include "VerletObj.h"
#include "QuadTree.h"

inline sf::Color get_color(int & step, const int radius,
                           const sf::Color prev)
{
    int r = prev.r, g = prev.g, b = prev.b;
    switch (step)
    {
        case 0: // R --> O --> Y
            g += radius;
            if (g >= 255)
            {
                g = 255;
                ++step;
            }
            break;
        case 1: // Y --> G
            r -= radius;
            if (r <= 0)
            {
                r = 0;
                ++step;
            }
            break;
        case 2: // G --> B
            g -= radius;
            b += radius;
            if (b >= 255)
            {
                b = 255;
                g = 0;
                ++step;
            }
            break;
        case 3: // B --> V --> R
            b -= radius;
            r += radius;
            if (r >= 255)
            {
                b = 0;
                r = 255;
                step = 0;
            }
            break;
    };
    
    return {sf::Uint8(r), sf::Uint8(g), sf::Uint8(b)};
}

class Window_Not_Set_Error{};

class Simulation
{
public:
    Simulation(const sf::Vector2f gravity,
               const int sub_steps) :
        gravity_(gravity),
        sub_steps_(sub_steps),
        quad_tree_(nullptr)
    {
        bounds_.setRadius(WIDTH / 2);
        bounds_.setOrigin(WIDTH / 2, WIDTH / 2);
        bounds_.setPosition(WIDTH / 2, HEIGHT / 2);
        bounds_.setFillColor(sf::Color::Black);

        quad_tree_ = new QuadTree(0, 0,
                                  WIDTH, HEIGHT,
                                  5, 5, // threshold = 5, max_depth = 5
                                  &objects_);
    }

    ~Simulation()
    {
        for (VerletObj * & v_o : objects_)
            delete v_o;
        
        delete quad_tree_;
    }

    void update_objects(const float dt);
    void move_objects(const float dt);
    void kill_out_of_bounds();
    void apply_gravity();
    void apply_constraint();
    void adjust_for_collisions();
    void draw() const;
    void create_obj();
    void create_obj_spray();
    
    static
    void set_window(sf::RenderWindow * w_ptr)
    { w_ptr_ = w_ptr; }
    
private:
    static sf::RenderWindow * w_ptr_;
    sf::Vector2f gravity_;
    int sub_steps_;

    sf::CircleShape bounds_;
    std::vector< VerletObj* > objects_;

    QuadTree * quad_tree_;
};

#endif
