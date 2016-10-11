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


#ifndef JIMLIB_SEARCH_HPP
#define JIMLIB_SEARCH_HPP

#include "Image/GenericPixel.hpp"

namespace jimlib
{
    template<class Pixel>
    typename Pixel::Type min(const GenericImage<Pixel> &Src)
    {
        auto it = Src.begin();
        typename Pixel::Type min_ = it[0];
        for (; it != Src.end(); ++it)
        {
            for (int32_t p = 0; p < Pixel::Plants; ++p)
            {
                if (it[p] < min_)
                {
                    min_ = it[p];
                }
            }
        }
        return min_;
    }
    template<class Pixel>
    typename Pixel::Type max(const GenericImage<Pixel> &Src)
    {
        auto it = Src.begin();
        typename Pixel::Type max_ = it[0];
        for (; it != Src.end(); ++it)
        {
            for (int32_t p = 0; p < Pixel::Plants; ++p)
            {
                if (it[p] > max_)
                {
                    max_ = it[p];
                }
            }
        }
        return max_;
    }
    
    template<typename AccumulatorT, class Pixel>
    typename Pixel::Type mean(const GenericImage<Pixel> &Src)
    {
        auto it = Src.begin();
        AccumulatorT sum = 0;
        int32_t count = 0;
        for (; it != Src.end(); ++it)
        {
            for (int32_t p = 0; p < Pixel::Plants; ++p)
            {
                sum += it[p];
                ++count;
            }
        }
        typename Pixel::Type res = 0;
        if (count > 0)
        {
            res = (typename Pixel::Type)(sum/count);
        }
        return res;
    }
};
#endif //JIMLIB_SEARCH_HPP
