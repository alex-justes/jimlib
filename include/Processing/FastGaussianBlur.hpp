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
    static void Blur(GenericImage<Pixel> *Image, double Sigma);
private:
    class DoubleSum : public GenericPixel<double, 1> {};
    template<uint8_t Passes>
    static void CalculateBoxSizes(double Sigma, uint32_t (&Sizes)[Passes]);
    static void HorizontalBlur(const GenericImage<DoubleSum> &Src, GenericImage<DoubleSum> &HSum, uint32_t R);
    static void VerticalBlur  (const GenericImage<DoubleSum> &Src, GenericImage<DoubleSum> &VSum, uint32_t R);
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

void FastGaussianBlur::HorizontalBlur(const GenericImage<DoubleSum> &Src, GenericImage<DoubleSum> &HSum, uint32_t R)
{
    uint32_t W = Src.GetWidth();
    uint32_t H = Src.GetHeight();
    HSum.Create(W, H);
    uint32_t r = (R-1)/2;
    for (uint32_t y = 0; y < H; ++y)
    {
        GenericImage<DoubleSum>::iterator it_l = Src.GetRow(y);
        GenericImage<DoubleSum>::iterator it_r = Src.GetRow(y);
        GenericImage<DoubleSum>::iterator it_blur = HSum.GetRow(y);
        double PartialSum = 0;
        for (uint32_t x = 0; x <= r; ++x, ++it_r)
        {
            PartialSum += it_r[0];
        }
        it_blur[0] = PartialSum / R;
        ++it_blur;
        for (uint32_t x = r + 1; x <= R - 1; ++x, ++it_r, ++it_blur)
        {
            PartialSum += it_r[0];
            it_blur[0] = PartialSum / R;
        }
        for (uint32_t x = R; x < W; ++x, ++it_r, ++it_l, ++it_blur)
        {
            PartialSum = PartialSum - it_l[0] + it_r[0];
            it_blur[0] = PartialSum / R;
        }
        for (uint32_t x = W - r; x < W; ++x, ++it_l, ++it_blur)
        {
            PartialSum = PartialSum - it_l[0];
            it_blur[0] = PartialSum / R;
        }
    }
}

void FastGaussianBlur::VerticalBlur(const GenericImage<DoubleSum> &Src, GenericImage<DoubleSum> &VSum, uint32_t R)
{
    uint32_t W = Src.GetWidth();
    uint32_t H = Src.GetHeight();
    assert(W <= MaxW);
    VSum.Create(W, H);
    uint32_t r = (R-1)/2;
    double PartialSum[MaxW];
    for (uint32_t i = 0; i < W; ++i)
    {
        PartialSum[i] = 0;
    }

    GenericImage<DoubleSum>::iterator it_b = VSum.begin();
    GenericImage<DoubleSum>::iterator it_d = Src.begin();
    GenericImage<DoubleSum>::iterator it_u = Src.begin();

    for (uint32_t y = 0; y <= r; ++y)
    {
        for (uint32_t x = 0; x < W; ++x, ++it_d)
        {
            PartialSum[x] += it_d[0];
        }
    }

    for (uint32_t x = 0; x < W; ++x, ++it_b)
    {
        it_b[0] = PartialSum[x] / R;
    }

    for (uint32_t y = r + 1; y <= R - 1; ++y)
    {
        for (uint32_t x = 0; x < W; ++x, ++it_d, ++it_b)
        {
            PartialSum[x] += it_d[0];
            it_b[0] = PartialSum[x] / R;
        }
    }

    for (uint32_t y = R; y < H; ++y)
    {
        for (uint32_t x = 0; x < W; ++x, ++it_d, ++it_u, ++it_b)
        {
            PartialSum[x] = PartialSum[x] - it_u[0] + it_d[0];
            it_b[0] = PartialSum[x] / R;
        }
    }

    for (uint32_t y = H - r; y < H; ++y)
    {
        for (uint32_t x = 0; x < W; ++x, ++it_u, ++it_b)
        {
            PartialSum[x] = PartialSum[x] - it_u[0];
            it_b[0] = PartialSum[x] / R;
        }
    }
}

template<uint8_t Passes, typename Pixel>
void FastGaussianBlur::Blur(GenericImage<Pixel> *Image, double Sigma)
{
    GenericImage<DoubleSum> HSum;
    GenericImage<DoubleSum> VSum;
    uint32_t Sizes[Passes];
    uint32_t W = Image->GetWidth();
    uint32_t H = Image->GetHeight();
    HSum.Create(W, H);
    VSum.Create(W, H);
    CalculateBoxSizes(Sigma, Sizes);
    for (uint16_t plant = 0; plant < Image->GetPlants(); ++plant)
    {
        // TODO: Copy is slow, we need to avoid that
        {
            GenericImage<DoubleSum>::iterator it_dst = VSum.begin();
            typename GenericImage<Pixel>::iterator it_src = Image->begin();
            for (; it_src != Image->end(); ++it_src, ++it_dst)
            {
                it_dst[0] = it_src[plant];
            }
        }
        for (uint16_t pass = 0; pass < Passes; ++pass)
        {
            HorizontalBlur(VSum, HSum, Sizes[pass]);
            VerticalBlur  (HSum, VSum, Sizes[pass]);
        }

        {
            GenericImage<DoubleSum>::iterator it_src = VSum.begin();
            typename GenericImage<Pixel>::iterator it_dst = Image->begin();
            for (; it_dst != Image->end(); ++it_src, ++it_dst)
            {
                it_dst[plant] = it_src[0];
            }
        }
    }
}

#endif //JIMLIB_FASTGAUSSIANBLUR_HPP
