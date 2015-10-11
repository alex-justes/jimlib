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

#ifndef JIMLIB_RECT_HPP
#define JIMLIB_RECT_HPP

#include <cstdint>

class Rect
{
public:
    Rect();
    Rect(int32_t top, int32_t left, int32_t bottom, int32_t right);
    bool Check(uint32_t W, uint32_t H);
    int32_t top;
    int32_t left;
    int32_t bottom;
    int32_t right;
};

// =======================================================

Rect::Rect()
        : top(0),
          left(0),
          bottom(0),
          right(0)
{ }

Rect::Rect(int32_t _top, int32_t _left, int32_t _bottom, int32_t _right)
        : top(_top),
          left(_left),
          bottom(_bottom),
          right(_right)
{ }

bool Rect::Check(uint32_t W, uint32_t H)
{
    bool A = left > 0 && right > 0 && top > 0 && bottom > 0;
    bool B = left < W && right < W && top < H && bottom < H;
    return (A && B);
}
#endif // JIMLIB_RECT_HPP