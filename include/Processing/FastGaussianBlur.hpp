//
// Created by justes on 10/10/15.
//

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
    class DoubleSum : public GenericPixel<double, 1> {};
    template<uint8_t Passes>
    static void CalculateBoxSizes(double Sigma, uint32_t (&Sizes)[Passes]);

    template<typename Pixel>
    static void HorizontalBlur(const GenericImage<Pixel> &Src, GenericImage<Pixel> &HSum, uint32_t R);
    template<typename Pixel>
    static void VerticalBlur  (const GenericImage<Pixel> &Src, GenericImage<Pixel> &VSum, uint32_t R);
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

template<typename Pixel>
void FastGaussianBlur::HorizontalBlur(const GenericImage<Pixel> &Src, GenericImage<Pixel> &HSum, uint32_t R)
{
    uint32_t W = Src.GetWidth();
    uint32_t H = Src.GetHeight();
    uint32_t r = (R-1)/2;

    for (uint16_t plant = 0; plant < Src.GetPlants(); ++plant)
    {
        for (uint32_t y = 0; y < H; ++y)
        {
            typename GenericImage<Pixel>::iterator it_l = Src.GetRow(y);
            typename GenericImage<Pixel>::iterator it_r = Src.GetRow(y);
            typename GenericImage<Pixel>::iterator it_blur = HSum.GetRow(y);
            double PartialSum = 0;
            for (uint32_t x = 0; x <= r; ++x, ++it_r)
            {
                PartialSum += it_r[plant];
            }
            it_blur[plant] = PartialSum / R;
            ++it_blur;
            for (uint32_t x = r + 1; x <= R - 1; ++x, ++it_r, ++it_blur)
            {
                PartialSum += it_r[plant];
                it_blur[plant] = PartialSum / R;
            }
            for (uint32_t x = R; x < W; ++x, ++it_r, ++it_l, ++it_blur)
            {
                PartialSum = PartialSum - it_l[plant] + it_r[plant];
                it_blur[plant] = PartialSum / R;
            }
            for (uint32_t x = W - r; x < W; ++x, ++it_l, ++it_blur)
            {
                PartialSum = PartialSum - it_l[plant];
                it_blur[plant] = PartialSum / R;
            }
        }
    }
}

template<typename Pixel>
void FastGaussianBlur::VerticalBlur(const GenericImage<Pixel> &Src, GenericImage<Pixel> &VSum, uint32_t R)
{
    uint32_t W = Src.GetWidth();
    uint32_t H = Src.GetHeight();
    assert(W <= MaxW);

    uint32_t r = (R-1)/2;
    double PartialSum[MaxW];

    for (uint16_t plant = 0; plant < Src.GetPlants(); ++plant)
    {
        typename GenericImage<Pixel>::iterator it_b = VSum.begin();
        typename GenericImage<Pixel>::iterator it_d = Src.begin();
        typename GenericImage<Pixel>::iterator it_u = Src.begin();

        for (uint32_t i = 0; i < W; ++i)
        {
            PartialSum[i] = 0;
        }

        for (uint32_t y = 0; y <= r; ++y)
        {
            for (uint32_t x = 0; x < W; ++x, ++it_d)
            {
                PartialSum[x] += it_d[plant];
            }
        }

        for (uint32_t x = 0; x < W; ++x, ++it_b)
        {
            it_b[plant] = PartialSum[x] / R;
        }

        for (uint32_t y = r + 1; y <= R - 1; ++y)
        {
            for (uint32_t x = 0; x < W; ++x, ++it_d, ++it_b)
            {
                PartialSum[x] += it_d[plant];
                it_b[plant] = PartialSum[x] / R;
            }
        }

        for (uint32_t y = R; y < H; ++y)
        {
            for (uint32_t x = 0; x < W; ++x, ++it_d, ++it_u, ++it_b)
            {
                PartialSum[x] = PartialSum[x] - it_u[plant] + it_d[plant];
                it_b[plant] = PartialSum[x] / R;
            }
        }

        for (uint32_t y = H - r; y < H; ++y)
        {
            for (uint32_t x = 0; x < W; ++x, ++it_u, ++it_b)
            {
                PartialSum[x] = PartialSum[x] - it_u[plant];
                it_b[plant] = PartialSum[x] / R;
            }
        }
    }
}

template<uint8_t Passes, typename Pixel>
void FastGaussianBlur::Blur(GenericImage<Pixel> &Image, double Sigma)
{
    GenericImage<Pixel> HSum;
    uint32_t Sizes[Passes];
    uint32_t W = Image.GetWidth();
    uint32_t H = Image.GetHeight();
    HSum.Create(W, H);
    CalculateBoxSizes(Sigma, Sizes);
    for (uint16_t pass = 0; pass < Passes; ++pass)
    {
        HorizontalBlur(Image, HSum, Sizes[pass]);
        VerticalBlur  (HSum, Image, Sizes[pass]);
    }
}

#endif //JIMLIB_FASTGAUSSIANBLUR_HPP
