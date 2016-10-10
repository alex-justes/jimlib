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

#ifndef JIMLIB_CONVOLUTION2D_HPP
#define JIMLIB_CONVOLUTION2D_HPP

#include <memory>
#include "Image/GenericImage.hpp"
namespace jimlib
{
    template<typename OutputT, typename InputT>
    class Convolution2D : public GenericImage<GenericPixel<OutputT, 1>>
    {
    public:
        template<typename Pixel>
        void convolve3_horizontal(const GenericImage<Pixel> &Src, InputT k1, InputT k2, InputT k3);
        void convolve3_horizontal(InputT k1, InputT k2, InputT k3);
        template<typename Pixel>
        void convolve3_vertical(const GenericImage<Pixel> &Src, InputT k1, InputT k2, InputT k3);
        void convolve3_vertical(InputT k1, InputT k2, InputT k3);
    };
    
    template<typename OutputT, typename InputT>
    template<typename Pixel>
    void Convolution2D<OutputT, InputT>::convolve3_horizontal(const GenericImage<Pixel> &Src, InputT k1, InputT k2, InputT k3)
    {
        static_assert(Pixel::Plants == 1, "Only 1-plant images are allowed");
        int32_t W = Src.GetWidth();
        int32_t H = Src.GetHeight();
        this->Create(W,H);
        auto it_src = Src.begin();
        auto it_dst = this->begin();
        // TODO: check W
        for (int32_t y = 0; y < H; ++y)
        {
            it_dst[0] = k2*it_src[0] + k3*it_src[1];
            ++it_src;
            ++it_dst;
            for (int32_t x = 1; x < W - 1; ++x)
            {
                it_dst[0] = k1*it_src[-1] + k2*it_src[0] + k3*it_src[1];
                ++it_src;
                ++it_dst;
            }
            it_dst[0] = k1*it_src[-1] + k2*it_src[0];
            ++it_src;
            ++it_dst;
        }
    }
    
    template<typename OutputT, typename InputT>
    void Convolution2D<OutputT, InputT>::convolve3_horizontal(InputT k1, InputT k2, InputT k3)
    {
        int32_t W = this->GetWidth();
        int32_t H = this->GetHeight();
        if (W < 1 || H < 1)
        {
            return;
        }
        auto it = this->begin();
        // TODO: check W
        for (int32_t y = 0; y < H; ++y)
        {
            OutputT buf = it[0];
            it[0] = k2*it[0] + k3*it[1];
            ++it;
            for (int32_t x = 1; x < W - 1; ++x)
            {
                OutputT tmp = it[0];
                it[0] = k1*buf + k2*it[0] + k3*it[1];
                ++it;
                buf = tmp;
            }
            it[0] = k1*buf + k2*it[0];
            ++it;
        }
    }
    
    
    template<typename OutputT, typename InputT>
    template<typename Pixel>
    void Convolution2D<OutputT, InputT>::convolve3_vertical(const GenericImage<Pixel> &Src, InputT k1, InputT k2, InputT k3)
    {
        static_assert(Pixel::Plants == 1, "Only 1-plant images are allowed");
        int32_t W = Src.GetWidth();
        int32_t H = Src.GetHeight();
        this->Create(W, H);
        auto it_src_1 = Src.begin();
        auto it_src_2 = Src.begin();
        auto it_src_3 = Src.GetRow(1);
        auto it_dst = this->begin();
        // TODO: check W
        for (int32_t x = 0; x < W; ++x, ++it_dst, ++it_src_2, ++it_src_3)
        {
            it_dst[0] = k2*it_src_2[0] + k3*it_src_3[0];
        }
        for (int32_t y = 1; y < H - 1; ++y)
        {
            for (int32_t x = 0; x < W; ++x, ++it_dst, ++it_src_1, ++it_src_2, ++it_src_3)
            {
                it_dst[0] = k1*it_src_1[0] + k2*it_src_2[0] + k3*it_src_3[0];
            }
        }
        for (int32_t x = 0; x < W; ++x, ++it_dst, ++it_src_1, ++it_src_2)
        {
            it_dst[0] = k1*it_src_1[0] + k2*it_src_2[0];
        }
    }
    
    template<typename OutputT, typename InputT>
    void Convolution2D<OutputT, InputT>::convolve3_vertical(InputT k1, InputT k2, InputT k3)
    {
        int32_t W = this->GetWidth();
        int32_t H = this->GetHeight();
        if (W < 1 || H < 1)
        {
            return;
        }
        std::unique_ptr<OutputT[]> buf(new OutputT[W]);
        auto it_2 = this->begin();
        auto it_3 = this->GetRow(1);
        for (int32_t x = 0; x < W; ++x, ++it_2, ++it_3)
        {
            buf[x] = it_2[0];
            it_2[0] = k2*it_2[0] + k3*it_3[0];
        }
        for (int32_t y = 1; y < H - 1; ++y)
        {
            for (int32_t x = 0; x < W; ++x, ++it_2, ++it_3)
            {
                OutputT tmp = it_2[0];
                it_2[0] = k1*buf[x] + k2*it_2[0] + k3*it_3[0];
                buf[x] = tmp;
            }
        }
        for (int32_t x = 0; x < W; ++x, ++it_2, ++it_3)
        {
            it_2[0] = k1*buf[x] + k2*it_2[0];
        }
    }
}

#endif //JIMLIB_CONVOLUTION2D_HPP
