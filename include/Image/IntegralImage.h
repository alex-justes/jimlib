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

#ifndef INTEGRALIMAGE_H
#define INTEGRALIMAGE_H

#include <cstdint>
#include <cstring>
#include <cassert>
#include "GenericImage.h"
#include "PixelTypes.h"
#include "Utils/Rect.h"

using namespace std;

class IntegralImage : public GenericImage<PixelType::Mono64>
{
public:
    void Calculate(const GenericImage<PixelType::Mono8> &Src);
    void CalculateSquared(const GenericImage<PixelType::Mono8> &Src);
    uint64_t GetSum(const Rect &rc) const;
    uint64_t GetSumUnsafe(const Rect &rc) const;
    uint64_t GetFullSumUnsafe(uint32_t x, uint32_t y) const;
private:
    uint64_t GetSum(int32_t x, int32_t y) const;
};

#endif //INTEGRALIMAGE_H

