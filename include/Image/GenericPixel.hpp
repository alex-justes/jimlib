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

#ifndef JIMLIB_GENERICPIXEL_H
#define JIMLIB_GENERICPIXEL_H

#include <cstdint>
#include <cassert>

template<typename T, uint8_t PlantsAmount>
class GenericPixel
{
public:
    T &operator[](uint8_t Plant);
    const T &operator[](uint8_t Plant) const;
    static const uint8_t Plants = PlantsAmount;
    static const uint32_t SizeOfPixel = sizeof(T) * PlantsAmount;
    typedef T Type;
    typedef GenericPixel<T, PlantsAmount> ParentType;
    T m_Buffer[PlantsAmount];
};

// =======================================================

template<typename T, uint8_t PlantsAmount>
T &GenericPixel<T, PlantsAmount>::operator[](uint8_t Plant)
{
    assert(Plant < Plants);
    return m_Buffer[Plant];
}

template<typename T, uint8_t PlantsAmount>
const T &GenericPixel<T, PlantsAmount>::operator[](uint8_t Plant) const
{
    assert(Plant < Plants);
    return m_Buffer[Plant];
}

#endif //JIMLIB_GENERICPIXEL_H