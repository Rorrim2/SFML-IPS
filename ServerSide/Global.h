#pragma once
#include <iostream>
#define DEBUG 1

#define DEBUG_COUT(msg) if(DEBUG == 1) std::cerr << msg << std::endl; 
#define DELLISNOTNULL(object) if(object != nullptr){ delete object; object = nullptr;}

// 1 meter (box2d) is more or less 64 pixels (sfml)
#define PIXELS_PER_METER 30.0f

// 64 pixels (sfml) are more or less 1 meter (box2d)
#define METERS_PER_PIXEL (1.0F/PIXELS_PER_METER)