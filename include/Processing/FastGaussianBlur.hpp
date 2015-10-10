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

#ifndef JIMLIB_FASTGAUSSIANBLUR_HPP
#define JIMLIB_FASTGAUSSIANBLUR_HPP

#include <cmath>
#include "Image/GenericImage.hpp"
#include "Image/PixelTypes.hpp"

class FastGaussianBlur
{
public:
    static const uint32_t MaxW = 10000;
    template<uint8_t Passes, typename Pixel>
    static void Blur(GenericImage<Pixel> &Image, double Sigma);
private:
    template<uint8_t Passes>
    static void CalculateBoxSizes(double Sigma, uint32_t (&Sizes)[Passes]);
    template<typename PixelSrc, typename PixelDst>
    static void HorizontalBlur(const GenericImage<PixelSrc> &Src, GenericImage<PixelDst> &HSum, uint32_t R);
    template<typename PixelSrc, typename PixelDst>
    static void VerticalBlur(const GenericImage<PixelSrc> &Src, GenericImage<PixelDst> &VSum, uint32_t R);
};

template<uint8_t Passes>
void FastGaussianBlur::CalculateBoxSizes(double Sigma, uint32_t (&Sizes)[Passes])
{
    double FW = sqrt((12.0*Sigma*Sigma / Passes) + 1);
    uint32_t l = (uint32_t)floor(FW);
    if (!(l&1))
    {
        l -= 1;
    }
    uint32_t u = l + 2;
    double FM = round((12.0*Sigma*Sigma - Passes*l*l - 4*Passes*l - 3*Passes)/(-4.0*l - 4));
    for (uint16_t i = 0; i < Passes; ++i)
    {
        Sizes[i] = (i < FM) ? l : u;
    }
}

template<typename PixelSrc, typename PixelDst>
void FastGaussianBlur::HorizontalBlur(const GenericImage<PixelSrc> &Src, GenericImage<PixelDst> &HSum, uint32_t R)
{
    uint32_t W = Src.GetWidth();
    uint32_t H = Src.GetHeight();
    uint32_t r = (R-1)/2;

    typename GenericImage<PixelSrc>::iterator it_l = Src.begin();
    typename GenericImage<PixelSrc>::iterator it_r = Src.begin();
    typename GenericImage<PixelDst>::iterator it_blur = HSum.begin();

    for (uint32_t y = 0; y < H; ++y)
    {
        uint32_t PartialSum[GenericImage<PixelSrc>::Plants];
        memset(PartialSum, 0, sizeof(PartialSum));
        for (uint32_t x = 0; x <= r; ++x, ++it_r)
        {
            for (uint16_t plant = 0; plant < GenericImage<PixelSrc>::Plants; ++plant)
            {
                PartialSum[plant] += it_r[plant];
            }
        }
        for (uint16_t plant = 0; plant < GenericImage<PixelSrc>::Plants; ++plant)
        {
            it_blur[plant] = PartialSum[plant];
        }
        ++it_blur;
        for (uint32_t x = r + 1; x <= R - 1; ++x, ++it_r, ++it_blur)
        {
            for (uint16_t plant = 0; plant < GenericImage<PixelSrc>::Plants; ++plant)
            {
                PartialSum[plant] += it_r[plant];
                it_blur[plant] = PartialSum[plant];
            }
        }
        for (uint32_t x = R; x < W; ++x, ++it_r, ++it_l, ++it_blur)
        {
            for (uint16_t plant = 0; plant < GenericImage<PixelSrc>::Plants; ++plant)
            {
                PartialSum[plant] = PartialSum[plant] - it_l[plant] + it_r[plant];
                it_blur[plant] = PartialSum[plant];
            }
        }
        for (uint32_t x = W - r; x < W; ++x, ++it_l, ++it_blur)
        {
            for (uint16_t plant = 0; plant < GenericImage<PixelSrc>::Plants; ++plant)
            {
                PartialSum[plant] = PartialSum[plant] - it_l[plant];
                it_blur[plant] = PartialSum[plant];
            }
        }
        it_l += (R - r);
    }
}

template<typename PixelSrc, typename PixelDst>
void FastGaussianBlur::VerticalBlur(const GenericImage<PixelSrc> &Src, GenericImage<PixelDst> &VSum, uint32_t R)
{
    uint32_t W = Src.GetWidth();
    uint32_t H = Src.GetHeight();
    assert(W <= MaxW);

    uint32_t r = (R-1)/2;
    uint32_t PartialSum[GenericImage<PixelSrc>::Plants][MaxW];
    memset(PartialSum, 0, sizeof(PartialSum));
    double S = 1.0 / (R * R);

    typename GenericImage<PixelSrc>::iterator it_d = Src.begin();
    typename GenericImage<PixelSrc>::iterator it_u = Src.begin();
    typename GenericImage<PixelDst>::iterator it_b = VSum.begin();

    for (uint32_t y = 0; y <= r; ++y)
    {
        for (uint32_t x = 0; x < W; ++x, ++it_d)
        {
            for (uint16_t plant = 0; plant < GenericImage<PixelSrc>::Plants; ++plant)
            {
                PartialSum[plant][x] += it_d[plant];
            }
        }
    }
    for (uint32_t x = 0; x < W; ++x, ++it_b)
    {
        for (uint16_t plant = 0; plant < GenericImage<PixelSrc>::Plants; ++plant)
        {
            it_b[plant] = PartialSum[plant][x] * S;
        }
    }
    for (uint32_t y = r + 1; y <= R - 1; ++y)
    {
        for (uint32_t x = 0; x < W; ++x, ++it_d, ++it_b)
        {
            for (uint16_t plant = 0; plant < GenericImage<PixelSrc>::Plants; ++plant)
            {
                PartialSum[plant][x] += it_d[plant];
                it_b[plant] = PartialSum[plant][x] * S;
            }
        }
    }
    for (uint32_t y = R; y < H; ++y)
    {
        for (uint32_t x = 0; x < W; ++x, ++it_d, ++it_u, ++it_b)
        {
            for (uint16_t plant = 0; plant < GenericImage<PixelSrc>::Plants; ++plant)
            {
                PartialSum[plant][x] = PartialSum[plant][x] - it_u[plant] + it_d[plant];
                it_b[plant] = PartialSum[plant][x] * S;
            }
        }
    }
    for (uint32_t y = H - r; y < H; ++y)
    {
        for (uint32_t x = 0; x < W; ++x, ++it_u, ++it_b)
        {
            for (uint16_t plant = 0; plant < GenericImage<PixelSrc>::Plants; ++plant)
            {
                PartialSum[plant][x] = PartialSum[plant][x] - it_u[plant];
                it_b[plant] = PartialSum[plant][x] * S;
            }
        }
    }
}

template<uint8_t Passes, typename Pixel>
void FastGaussianBlur::Blur(GenericImage<Pixel> &Image, double Sigma)
{
    GenericImage<GenericPixel<uint16_t, GenericImage<Pixel>::Plants>> HSum;
    GenericImage<GenericPixel<uint16_t, GenericImage<Pixel>::Plants>> VSum;

    uint32_t Sizes[Passes];
    uint32_t W = Image.GetWidth();
    uint32_t H = Image.GetHeight();
    HSum.Create(W, H);
    VSum.Create(W, H);
    CalculateBoxSizes(Sigma, Sizes);

    HorizontalBlur(Image, HSum, Sizes[0]);
    VerticalBlur  (HSum, VSum, Sizes[0]);

    for (uint16_t pass = 1; pass < Passes; ++pass)
    {
        HorizontalBlur(VSum, HSum, Sizes[pass]);
        VerticalBlur  (HSum, VSum, Sizes[pass]);
    }

    typename GenericImage<GenericPixel<uint16_t, GenericImage<Pixel>::Plants>>::iterator it_src = VSum.begin();
    typename GenericImage<Pixel>::iterator it_dst = Image.begin();
    for (; it_src != VSum.end(); ++it_src, ++it_dst)
    {
        for (uint16_t p = 0; p < GenericImage<Pixel>::Plants; ++p)
        {
            it_dst[p] = it_src[p];
        }
    }
}

#endif //JIMLIB_FASTGAUSSIANBLUR_HPP
