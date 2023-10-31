#ifndef VERLET_OBJ_H
#define VERLET_OBJ_H

#include "Common.h"

class VerletObj : public sf::CircleShape
{
public:
    VerletObj(const float r, const float x, const float y,
              const sf::Color c = sf::Color::White) :
        sf::CircleShape(r)
    {
        setOrigin(r, r);
        setPosition(x, y);
        old_pos_ = getPosition();
        setFillColor(c);
    }

    void update(const float dt)
    {
        sf::Vector2f velocity = getPosition() - old_pos_;
        old_pos_ = getPosition();

        //Verlet integration
        setPosition(old_pos_ + velocity + accel_ * (dt * dt));
        
        accel_ = {0, 0};

        return;
    }

    inline
    void accelerate(sf::Vector2f accel)
    { accel_ += accel; }

    bool collides(const VerletObj & v_o) const
    {
        sf::Vector2f to_obj = getPosition() - v_o.getPosition();
        float len = sqrt(to_obj.x * to_obj.x + to_obj.y * to_obj.y);
        
        return len < getRadius() + v_o.getRadius();
    }
    
private:
    sf::Vector2f old_pos_;
    sf::Vector2f accel_;
};

#endif
