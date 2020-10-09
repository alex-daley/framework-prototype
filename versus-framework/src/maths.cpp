#pragma once
#include "maths.h"
#include <cmath>

namespace vsf
{
    int maths::round_to_int(float value)
    {
        return static_cast<int>(std::round(value));
    }

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

    RectInt::RectInt(int min_x, int min_y, int max_x, int max_y) :
        min_x(min_x),
        min_y(min_y),
        max_x(max_x),
        max_y(max_y)
    {
    }
    
    RectInt::RectInt() : RectInt(0, 0, 0, 0)
    {
    }
    
    int RectInt::width() const
    {
        return max_x - min_x;
    }
    
    int RectInt::height() const
    {
        return max_y - min_y;
    }

    Vector2::Vector2(float x, float y) : x(x), y(y)
    {
    }

    Vector2::Vector2() : Vector2(0.0f, 0.0f)
    {
    }
}

