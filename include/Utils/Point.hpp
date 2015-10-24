/*
 *  jimlib -- generic image and-image algorithms library
 *  Copyright (C) 2015 Alexey Titov
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:

 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 *
 *  Alexey Titov
 *  alex.justes@gmail.com
 *  https://github.com/alex-justes/jimlib
 */

#ifndef JIMLIB_POINT_HPP
#define JIMLIB_POINT_HPP
#include <cstdint>
namespace jimlib
{
    class Point
    {
    public:
        Point();
        Point(int32_t x, int32_t y);
        Point(const Point &point);
        int32_t x;
        int32_t y;
    };

    // =======================================================

    inline Point::Point()
    : x(0), y(0)
    { }
    inline Point::Point(int32_t _x, int32_t _y)
    : x(_x), y(_y)
    { }
    inline Point::Point(const Point &point)
    : x(point.x), y(point.y)
    { }
};


#endif //JIMLIB_POINT_HPP
