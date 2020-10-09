#pragma once

namespace vsf
{
    namespace maths
    {
        int round_to_int(float value);
    }

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

    struct RectInt
    {
        RectInt(int min_x, int min_y, int max_x, int max_y);
        RectInt();

        int width() const;
        int height() const;

        int min_x;
        int min_y;
        int max_x;
        int max_y;
    };

    struct Vector2
    {
        Vector2(float x, float y);
        Vector2();

        float x;
        float y;
    };
}
