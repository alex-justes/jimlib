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

#ifndef JIMLIB_CANNY_HPP
#define JIMLIB_CANNY_HPP

#include <cmath>
#include <set>
#include "Image/BinaryImage.hpp"
#include "Image/GrayImage.hpp"
#include "EdgeDetection/Sobel.hpp"
#include "Processing/Cluster.hpp"

namespace jimlib
{
    class Canny
    {
    public:
        void Calculate(const GenericImage<PixelType::Mono8> &Src, BinaryImage &Dst, int32_t T1, int32_t T2, float sigma1 = 0.33, float sigma2 = 0.33);
    private:
        static constexpr int32_t FACTOR = 100;
        static constexpr int32_t TH1 = (int32_t)(FACTOR*tan(M_PI/8));
        static constexpr int32_t TH2 = (int32_t)(FACTOR*tan(M_PI/8 + M_PI/4));
        BinaryImage m_nonMaxSuppressed;
        GrayImage m_Mass;
        Cluster m_Clusters;
        std::set<int16_t> m_Lookup;
    };
    
    void Canny::Calculate(const GenericImage<PixelType::Mono8> &Src, BinaryImage &Dst, int32_t T1, int32_t T2, float sigma1, float sigma2)
    {
        assert(T2 > T1);
        bool Auto = false;
        if (T1 == 0 && T2 == 0)
        {
            Auto = true;
        }

    
        int32_t W = Src.GetWidth();
        int32_t H = Src.GetHeight();
        m_nonMaxSuppressed.Create(W,H,PixelType::Mono8(0));
        m_Mass.Create(W,H,PixelType::Mono8(0));
        
        Dst.Create(W,H,PixelType::Mono8(0));
        Sobel grad;
        grad.Calculate(Src, 255);
        auto gx = grad.GetGx();
        auto gy = grad.GetGy();
        auto magn = grad.GetMagnitude();
        if (Auto)
        {
            int32_t mean_ = mean<int32_t>(*magn);
            T1 = (int32_t)(max(0.0, (1.0 - sigma1) * mean_));
            T2 = (int32_t)(min(255.0, (1.0 + sigma2) * mean_));
        }
        constexpr int32_t indices[][4] = {{0,-1,0,1}, {-1,0,1,0},{-1,-1,1,1},{1,-1,-1,1}};
        for (int32_t y = 1; y < H - 1; ++y)
        {
            auto it_gx = gx->GetColRow(1, y);
            auto it_gy = gy->GetColRow(1, y);
            auto it_m1 = magn->GetColRow(1, y - 1);
            auto it_m2 = magn->GetColRow(1, y);
            auto it_m3 = magn->GetColRow(1, y + 1);
            auto it_dst = m_nonMaxSuppressed.GetColRow(1,y);
            const GenericImage<GenericPixel<int32_t , 1>>::const_iterator *rows[] = {&it_m1, &it_m2, &it_m3};
            for (int32_t x = 1; x < W - 1; ++x, ++it_gx, ++it_gy, ++it_m1, ++it_m2, ++it_m3, ++it_dst)
            {
                int32_t sign = TH2 + FACTOR;
                if (it_gx[0] > 0)
                {
                    sign = (FACTOR * it_gy[0]) / it_gx[0];
                }
                int32_t angle = abs(sign);
                const int32_t *ind = nullptr;
                if (angle > TH2)
                {
                    ind = indices[0];
                }
                else if (angle < TH1)
                {
                    ind = indices[1];
                }
                else if (sign > 0)
                {
                    ind = indices[2];
                }
                else
                {
                    ind = indices[3];
                }
                const GenericImage<GenericPixel<int32_t , 1>>::const_iterator *r1 = rows[ind[1] + 1];
                const GenericImage<GenericPixel<int32_t , 1>>::const_iterator *r2 = rows[ind[3] + 1];
                if (it_m2[0] <= (*r1)[ind[0]] || it_m2[0] <= (*r2)[ind[2]])
                {
                    it_dst[0] = 0;
                }
                else if (it_m2[0] == (*r1)[ind[0]] && it_m2[0] == (*r2)[ind[2]] && it_m2[0] > T2)
                {
                    it_dst[0] = 1;
                }
                else if (it_m2[0] > T1)
                {
                    it_dst[0] = 1;
                }
            }
        }
        
        auto it_mask = m_nonMaxSuppressed.begin();
        auto it_mass = m_Mass.begin();
        auto it_magn = magn->begin();
        
        for (; it_mass != m_Mass.end(); ++it_magn, ++it_mask, ++it_mass)
        {
            if (it_mask[0] > 0)
            {
                if (it_magn[0] > T2 || it_magn[0] > T1)
                {
                    it_mask[0] = 1;
                }
                else
                {
                    it_mask[0] = 0;
                }
                if (it_magn[0] > T2)
                {
                    it_mass[0] = 1;
                }
            }
        }
        int16_t clusterAmount = m_Clusters.ClusterizeMask(m_Mass, m_nonMaxSuppressed);
        m_Lookup.clear();
        for (int16_t i = 0; i < clusterAmount; ++i)
        {
            const ClusterItem &item = m_Clusters.GetCluster(i);
            if (item.Mass > 0)
            {
                m_Lookup.insert(i);
            }
        }
        auto it_src = m_Clusters.begin();
        auto it_dst = Dst.begin();
        for (; it_dst != Dst.end(); ++it_dst, ++it_src)
        {
            auto search = m_Lookup.find(it_src[0]);
            if (search != m_Lookup.end())
            {
                it_dst[0] = 1;
            }
        }
    }

}

#endif //JIMLIB_CANNY_HPP
