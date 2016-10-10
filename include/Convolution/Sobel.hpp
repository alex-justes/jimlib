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
 *
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

#ifndef JIMLIB_SOBEL_HPP
#define JIMLIB_SOBEL_HPP

#include <cmath>
#include "Convolution/Convolution.hpp"

namespace jimlib
{
    template<typename OutputT, typename InputT>
    class Sobel
    {
    public:
        template<class Pixel>
        void Calculate(const GenericImage<Pixel> &Src);
        const Convolution2D<OutputT, InputT> *GetGx() const;
        const Convolution2D<OutputT, InputT> *GetGy() const;
        const GenericImage<GenericPixel<OutputT, 1>> *GetMagnitude() const;
    private:
        Convolution2D<OutputT, InputT> m_Gx;
        Convolution2D<OutputT, InputT> m_Gy;
        GenericImage<GenericPixel<OutputT, 1>> m_Magnitude;
    };
    
    template<typename OutputT, typename InputT>
    template<class Pixel>
    void Sobel<OutputT, InputT>::Calculate(const GenericImage<Pixel> &Src)
    {
        int32_t W = Src.GetWidth();
        int32_t H = Src.GetHeight();
        m_Gx.convolve3_horizontal(Src, 1, 0, -1);
        m_Gx.convolve3_vertical(1, 2, 1);
        m_Gy.convolve3_horizontal(Src, 1, 2, 1);
        m_Gy.convolve3_vertical(1, 0, -1);
        m_Magnitude.Create(W,H);
        
        auto it_m = m_Magnitude.begin();
        auto it_gx = m_Gx.begin();
        auto it_gy = m_Gy.begin();
        for (; it_m != m_Magnitude.end(); ++it_m, ++it_gx, ++it_gy)
        {
            it_m[0] = (OutputT)sqrt(it_gx[0]*it_gx[0] + it_gy[0]*it_gy[0]);
        }
    }
    
    template<typename OutputT, typename InputT>
    const Convolution2D<OutputT, InputT> *Sobel<OutputT, InputT>::GetGx() const
    {
        return &m_Gx;
    }
    
    template<typename OutputT, typename InputT>
    const Convolution2D<OutputT, InputT> *Sobel<OutputT, InputT>::GetGy() const
    {
        return &m_Gy;
    }
    
    template<typename OutputT, typename InputT>
    const GenericImage<GenericPixel<OutputT, 1>> *Sobel<OutputT, InputT>::GetMagnitude() const
    {
        return &m_Magnitude;
    }
}

#endif //JIMLIB_SOBEL_HPP
