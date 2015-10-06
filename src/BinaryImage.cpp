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

#include "Image/BinaryImage.h"
#include "Image/IntegralImage.h"
#include <cmath>
#include <cstring>

void BinaryImage::Threshold(const GrayImage &Src, uint8_t Threshold)
{
    uint32_t W = Src.GetWidth();
    uint32_t H = Src.GetHeight();
    Create(W, H, PixelType::Mono8(0));
    GrayImage::iterator it_src = Src.begin();
    BinaryImage::iterator it_dst = begin();
    for (; it_src != Src.end(); ++it_src, ++it_dst)
    {
        if (it_src[0] > Threshold)
        {
            it_dst[0] = 1;
        }
        else
        {
            it_dst[0] = 0;
        }
    }
}

void BinaryImage::Otsu(const GrayImage &Src)
{
    GrayImage::iterator it_src = Src.begin();
    uint32_t Histo[256];
    memset(Histo, 0, sizeof(Histo));
    for (; it_src != Src.end(); ++it_src)
    {
        ++Histo[it_src[0]];
    }
    uint32_t Energy = 0;
    uint32_t Sum = 0;
    for (uint32_t i = 0; i <= 255; ++i)
    {
        Energy += i * Histo[i];
        Sum += Histo[i];
    }
    uint32_t PartialEnergy = 0;
    uint32_t PartialSum = 0;
    uint32_t Threshold = 0;
    double w1 = 0;
    double a = 0;
    double Sigma = 0;
    double MaxSigma = -1;
    for (uint32_t i = 0; i <= 255; ++i)
    {
        PartialEnergy += i * Histo[i];
        PartialSum += Histo[i];
        w1 = (double)PartialSum / Sum;
        a = (double)PartialEnergy / PartialSum - (double)(Energy - PartialEnergy) / (Sum - PartialSum);
        Sigma = w1*(1 - w1)*a*a;
        if (Sigma > MaxSigma)
        {
            MaxSigma = Sigma;
            Threshold = i;
        }
    }
    Create(Src.GetWidth(), Src.GetHeight(), PixelType::Mono8(0));
    BinaryImage::iterator it_dst = begin();
    it_src = Src.begin();
    for (; it_src != Src.end(); ++it_src, ++it_dst)
    {
        if (it_src[0] > Threshold)
        {
            it_dst[0] = 1;
        }
        else
        {
            it_dst[0] = 0;
        }
    }
}

void BinaryImage::Niblack(const GrayImage &Src, uint32_t WindowSize, double K)
{
    IntegralImage Mean;
    IntegralImage Mean2;
    Mean.Calculate(Src);
    Mean2.CalculateSquared(Src);
    uint32_t W = Src.GetWidth();
    uint32_t H = Src.GetHeight();
    Create(W, H, PixelType::Mono8(0));
    GrayImage::iterator it_src = Src.begin();
    BinaryImage::iterator it_dst = begin();
    double Sq = (1 + WindowSize) * (1 + WindowSize);
    for (uint32_t y = 0; y < H; ++y)
    {
        for (uint32_t x = 0; x < W; ++x, ++it_src, ++it_dst)
        {
            Rect rc(y - WindowSize/2, x - WindowSize/2, y + WindowSize/2, x + WindowSize/2);
            double Mx = Mean.GetSum(rc) / Sq;
            double Mx2 = Mean2.GetSum(rc) / Sq;
            double StdDev = sqrt(Mx2 - Mx*Mx);
            double Threshold = Mx + K * StdDev;
            if (it_src[0] > Threshold)
            {
                it_dst[0] = 1;
            }
            else
            {
                it_dst[0] = 0;
            }
        }
    }
}

