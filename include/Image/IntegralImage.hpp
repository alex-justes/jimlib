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

#ifndef JIMLIB_INTEGRALIMAGE_HPP
#define JIMLIB_INTEGRALIMAGE_HPP

#include "Image/GenericImage.hpp"
#include "Image/PixelTypes.hpp"
#include "Utils/Rect.hpp"
namespace jimlib
{
    class IntegralImage : public GenericImage<PixelType::Mono64>
    {
    public:
        template <typename Pixel>
        void Calculate(const GenericImage <Pixel> &Src);
        template <typename Pixel>
        void CalculateSquared(const GenericImage <Pixel> &Src);
        uint64_t GetSum(const Rect<int32_t> &rc) const;
        uint64_t GetSumUnsafe(const Rect<int32_t> &rc) const;
        uint64_t GetFullSumUnsafe(uint32_t x, uint32_t y) const;
    private:
        uint64_t GetSum(int32_t x, int32_t y) const;
    };

// =======================================================

    inline uint64_t IntegralImage::GetSum(int32_t x, int32_t y) const
    {
        if (x < 0 || y < 0)
            return 0;
        x = ((uint32_t)x >= GetWidth()) ? (GetWidth() - 1) : x;
        y = ((uint32_t)y >= GetHeight()) ? (GetHeight() - 1) : y;
        return GetPixel(x, y, 0);
    }

    inline uint64_t IntegralImage::GetSum(const Rect<int32_t> &rc) const
    {
        uint64_t A = GetSum(rc.right, rc.bottom);
        uint64_t B = GetSum(rc.left - 1, rc.top - 1);
        uint64_t C = GetSum(rc.right, rc.top - 1);
        uint64_t D = GetSum(rc.left - 1, rc.bottom);
        return (A + B - C - D);
    }

    inline uint64_t IntegralImage::GetSumUnsafe(const Rect<int32_t> &rc) const
    {
        return (GetPixel(rc.right, rc.bottom, 0)
                + GetPixel(rc.left - 1, rc.top - 1, 0)
                - GetPixel(rc.right, rc.top - 1, 0)
                - GetPixel(rc.left - 1, rc.bottom, 0)
        );
    }

    inline uint64_t IntegralImage::GetFullSumUnsafe(uint32_t x, uint32_t y) const
    {
        return GetPixel(x, y, 0);
    }

    template <typename Pixel>
    void IntegralImage::Calculate(const GenericImage<Pixel> &Src)
    {
        uint32_t W = Src.GetWidth();
        uint32_t H = Src.GetHeight();
        Create(W, H);
        typename GenericImage<Pixel>::const_iterator it_src = Src.begin();
        IntegralImage::iterator it_dst = begin();
        it_dst[0] = it_src[0];
        ++it_dst;
        ++it_src;
        IntegralImage::iterator it_sum = begin();
        for (uint32_t x = 1; x < W; ++x, ++it_dst, ++it_src, ++it_sum)
        {
            it_dst[0] = it_sum[0] + it_src[0];
        }
        it_sum = begin();

        for (uint32_t y = 1; y < H; ++y)
        {
            uint32_t LocalSum = 0;
            for (uint32_t x = 0; x < W; ++x, ++it_dst, ++it_src, ++it_sum)
            {
                LocalSum += it_src[0];
                it_dst[0] = it_sum[0] + LocalSum;
            }
        }
    }
    template <typename Pixel>
    void IntegralImage::CalculateSquared(const GenericImage<Pixel> &Src)
    {
        uint32_t W = Src.GetWidth();
        uint32_t H = Src.GetHeight();
        Create(W, H);
        typename GenericImage<Pixel>::const_iterator it_src = Src.begin();
        IntegralImage::iterator it_dst = begin();
        it_dst[0] = it_src[0] * it_src[0];
        ++it_dst;
        ++it_src;
        IntegralImage::iterator it_sum = begin();
        for (uint32_t x = 1; x < W; ++x, ++it_dst, ++it_src, ++it_sum)
        {
            it_dst[0] = it_sum[0] + it_src[0] * it_src[0];
        }
        it_sum = begin();
        for (uint32_t y = 1; y < H; ++y)
        {
            uint32_t LocalSum = 0;
            for (uint32_t x = 0; x < W; ++x, ++it_dst, ++it_src, ++it_sum)
            {
                LocalSum += (it_src[0] * it_src[0]);
                it_dst[0] = it_sum[0] + LocalSum;
            }
        }
    }
};

#endif //JIMLIB_INTEGRALIMAGE_HPP

