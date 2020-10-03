#pragma once
#include "maths.h"

namespace vsf
{
    Rect::Rect(float min_x, float min_y, float max_x, float max_y) :
        min_x(min_x),
        min_y(min_y), 
        max_x(max_x), 
        max_y(max_y)
    {
    }


    Rect::Rect() : Rect(0.0f, 0.0f, 0.0f, 0.0f)
    {
    }

    float Rect::width() const
    {
        return max_x - min_x;
    }
    
    float Rect::height() const
    {
        return max_y - min_y;
    }
}
