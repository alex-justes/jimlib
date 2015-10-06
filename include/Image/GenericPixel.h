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
 *    misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 *
 *  Alexey Titov
 *  alex.justes@gmail.com
 *  https://github.com/alex-justes/jimlib
 */

#ifndef GENERICPIXEL_H
#define GENERICPIXEL_H
#include <cstdint>

using namespace std;

template<typename T, uint8_t PlantsAmount>
class GenericPixel
{
public:
    virtual T &operator[](uint8_t Plant) = 0;
    virtual const T &operator[](uint8_t Plant) const = 0;
    static const uint8_t Plants = PlantsAmount;
    typedef T Type;
    typedef GenericPixel<T, PlantsAmount> ParentType;
};

#endif //GENERICPIXEL_H