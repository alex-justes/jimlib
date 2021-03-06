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

#ifndef JIMLIB_BINARYIMAGE_HPP
#define JIMLIB_BINARYIMAGE_HPP

#include <cmath>
#include "Image/IntegralImage.hpp"
namespace jimlib
{
    class BinaryImage : public GenericImage<PixelType::Mono8>
    {
    public:
        template <typename Pixel>
        void Otsu(const GenericImage<Pixel> &Src);
        template <typename Pixel>
        void Niblack(const GenericImage<Pixel> &Src, uint32_t WindowSize, double K);
        template <typename Pixel>
        void Sauvola(const GenericImage<Pixel> &Src, uint32_t WindowSize, double K);
        template <typename Pixel>
        void BoxMean(const GenericImage<Pixel> &Src, uint32_t WindowSize, double K);
        template <typename Pixel>
        void ThresholdUp(const GenericImage<Pixel> &Src, typename Pixel::Type Threshold);
        template <typename Pixel>
        void ThresholdDown(const GenericImage<Pixel> &Src, typename Pixel::Type Threshold);
        void Invert();
        void CopyFrom(const BinaryImage &Src);
        void CopyTo(BinaryImage &Dst) const;
    };

// =======================================================

    inline void BinaryImage::CopyFrom(const BinaryImage &Src)
    {
        CopyFromInternal(Src);
    }

    inline void BinaryImage::CopyTo(BinaryImage &Dst) const
    {
        CopyToInternal(Dst);
    }

    template <typename Pixel>
    void BinaryImage::ThresholdUp(const GenericImage<Pixel> &Src, typename Pixel::Type Threshold)
    {
        static_assert(GenericImage<Pixel>::Plants == 1, "Binary image supports only images with 1 plant.");
        uint32_t W = Src.GetWidth();
        uint32_t H = Src.GetHeight();
        Create(W, H, PixelType::Mono8(0));
        typename GenericImage<Pixel>::const_iterator it_src = Src.begin();
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
    template <typename Pixel>
    void BinaryImage::ThresholdDown(const GenericImage<Pixel> &Src, typename Pixel::Type Threshold)
    {
        static_assert(GenericImage<Pixel>::Plants == 1, "Binary image supports only images with 1 plant.");
        uint32_t W = Src.GetWidth();
        uint32_t H = Src.GetHeight();
        Create(W, H, PixelType::Mono8(0));
        typename GenericImage<Pixel>::const_iterator it_src = Src.begin();
        BinaryImage::iterator it_dst = begin();
        for (; it_src != Src.end(); ++it_src, ++it_dst)
        {
            if (it_src[0] < Threshold)
            {
                it_dst[0] = 1;
            }
            else
            {
                it_dst[0] = 0;
            }
        }
    }

    template <typename Pixel>
    void BinaryImage::Otsu(const GenericImage<Pixel> &Src)
    {
        static_assert(GenericImage<Pixel>::Plants == 1, "Binary image supports only images with 1 plant.");
        typename GenericImage<Pixel>::const_iterator it_src = Src.begin();
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
        typename Pixel::Type Threshold = 0;
        double w1 = 0;
        double a = 0;
        double Sigma = 0;
        double MaxSigma = -1;
        for (uint32_t i = 0; i <= 255; ++i)
        {
            PartialEnergy += i * Histo[i];
            PartialSum += Histo[i];
            w1 = (double) PartialSum / Sum;
            a = (double) PartialEnergy / PartialSum - (double) (Energy - PartialEnergy) / (Sum - PartialSum);
            Sigma = w1 * (1 - w1) * a * a;
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

    template <typename Pixel>
    void BinaryImage::Niblack(const GenericImage<Pixel> &Src, uint32_t WindowSize, double K)
    {
        static_assert(GenericImage<Pixel>::Plants == 1, "Binary image supports only images with 1 plant.");
        IntegralImage Mean;
        IntegralImage Mean2;
        Mean.Calculate(Src);
        Mean2.CalculateSquared(Src);
        uint32_t W = Src.GetWidth();
        uint32_t H = Src.GetHeight();
        Create(W, H, PixelType::Mono8(0));
        typename GenericImage<Pixel>::const_iterator it_src = Src.begin();
        BinaryImage::iterator it_dst = begin();
        double Sq = (1 + WindowSize) * (1 + WindowSize);
        for (uint32_t y = 0; y < H; ++y)
        {
            for (uint32_t x = 0; x < W; ++x, ++it_src, ++it_dst)
            {
                Rect<int32_t> rc(y - WindowSize / 2, x - WindowSize / 2, y + WindowSize / 2, x + WindowSize / 2);
                double Mx = Mean.GetSum(rc) / Sq;
                double Mx2 = Mean2.GetSum(rc) / Sq;
                double StdDev = sqrt(Mx2 - Mx * Mx);
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

    template <typename Pixel>
    void BinaryImage::Sauvola(const GenericImage<Pixel> &Src, uint32_t WindowSize, double K)
    {
        static_assert(GenericImage<Pixel>::Plants == 1, "Binary image supports only images with 1 plant.");
        IntegralImage Mean;
        IntegralImage StdDevIntegral;
        Mean.Calculate(Src);
        StdDevIntegral.CalculateSquared(Src);
        uint32_t W = Src.GetWidth();
        uint32_t H = Src.GetHeight();
        Create(W, H, PixelType::Mono8(0));
        typename GenericImage<Pixel>::const_iterator it_src = Src.begin();
        BinaryImage::iterator it_dst = begin();
        double Sq = (1 + WindowSize) * (1 + WindowSize);
        for (uint32_t y = 0; y < H; ++y)
        {
            for (uint32_t x = 0; x < W; ++x, ++it_src, ++it_dst)
            {
                Rect<int32_t> rc(y - WindowSize / 2, x - WindowSize / 2, y + WindowSize / 2, x + WindowSize / 2);
                double Mx = Mean.GetSum(rc) / Sq;
                double Mx2 = StdDevIntegral.GetSum(rc) / Sq;
                double StdDev = sqrt(Mx2 - Mx * Mx);
                double Threshold = Mx * (1 + K * (StdDev / (WindowSize / 2) - 1));
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
    
    template <typename Pixel>
    void BinaryImage::BoxMean(const GenericImage<Pixel> &Src, uint32_t WindowSize, double K)
    {
        static_assert(GenericImage<Pixel>::Plants == 1, "Binary image supports only images with 1 plant.");
        IntegralImage Mean;
        Mean.Calculate(Src);
        uint32_t W = Src.GetWidth();
        uint32_t H = Src.GetHeight();
        Create(W, H, PixelType::Mono8(0));
        typename GenericImage<Pixel>::const_iterator it_src = Src.begin();
        BinaryImage::iterator it_dst = begin();
        double Sq = (1 + WindowSize) * (1 + WindowSize);
        for (uint32_t y = 0; y < H; ++y)
        {
            for (uint32_t x = 0; x < W; ++x, ++it_src, ++it_dst)
            {
                Rect<int32_t> rc(y - WindowSize / 2, x - WindowSize / 2, y + WindowSize / 2, x + WindowSize / 2);
                double Mx = Mean.GetSum(rc) / Sq;
                double Threshold = Mx * K;
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

    inline void BinaryImage::Invert()
    {
        BinaryImage::iterator it = begin();
        for (; it != end(); ++it)
        {
            it[0] = it[0]^1;
        }
    }
};
#endif //JIMLIB_BINARYIMAGE_HPP
