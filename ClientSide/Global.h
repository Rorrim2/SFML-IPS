#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#define DEBUG 1

#define DEBUG_COUT(msg) if(DEBUG == 1) std::cerr << msg << std::endl; 
#define DELLISNOTNULL(object) if(object != nullptr){ delete object; object = nullptr;}

// 1 meter (box2d) is more or less 64 pixels (sfml)
#define PIXELS_PER_METER 30.0f

// 64 pixels (sfml) are more or less 1 meter (box2d)
#define METERS_PER_PIXEL (1.0F/PIXELS_PER_METER)

inline void lerp(sf::Vector2f& res, const sf::Vector2f& a, const sf::Vector2f& b, float t)
{
   res.x = (1 - t)*a.x + t * b.x;
   res.y = (1 - t)*a.y + t * b.y;
};