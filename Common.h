#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>

inline
std::ostream & operator<<(std::ostream & cout, const sf::Vector2f & v)
{
    cout << '(' << v.x << ", " << v.y << ')';
    return cout;
}

const int WIDTH = 800;
const int HEIGHT = 800;

#endif
