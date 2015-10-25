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
namespace jimlib
{
    template <typename T>
    class Rect
    {
    public:
        Rect();
        Rect(T top, T left, T bottom, T right);
        Rect(const Rect &Rc);
        T top;
        T left;
        T bottom;
        T right;
    };

// =======================================================

    template <typename T>
    Rect<T>::Rect()
            : top(0),
              left(0),
              bottom(0),
              right(0)
    { }

    template <typename T>
    Rect<T>::Rect(T _top, T _left, T _bottom, T _right)
            : top(_top),
              left(_left),
              bottom(_bottom),
              right(_right)
    { }

    template <typename T>
    Rect<T>::Rect(const Rect &Rc)
            : top(Rc.top),
              left(Rc.left),
              bottom(Rc.bottom),
              right(Rc.right)
    { }
};
#endif // JIMLIB_RECT_HPP