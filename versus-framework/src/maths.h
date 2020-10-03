#pragma once

namespace vsf
{
    struct Rect
    {
        Rect(float min_x, float min_y, float max_x, float max_y);
        Rect();

        float width() const;
        float height() const;

        float min_x;
        float min_y;
        float max_x;
        float max_y;
    };
}
