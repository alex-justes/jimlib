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

#ifndef JIMLIB_CLUSTER_HPP
#define JIMLIB_CLUSTER_HPP

#include <memory>
#include "Image/PixelTypes.hpp"
#include "Image/BinaryImage.hpp"

class ClusterItem
{
public:
    ClusterItem();
    uint32_t Mass;
    uint32_t SumX;
    uint32_t SumY;
    uint32_t Cx;
    uint32_t Cy;
    void CalculateCenter();
};

class Cluster : public GenericImage<PixelType::Mono16>
{
public:
    static const uint32_t MaxIdx = UINT16_MAX;
    static const uint32_t MaxClusters = UINT16_MAX;
    uint16_t Clusterize(const BinaryImage &Src);
    uint16_t GetClustersAmount() const;
    const ClusterItem &GetCluster(uint16_t idx) const;
private:
    ClusterItem m_Clusters[UINT16_MAX];
    uint16_t m_ClustersAmount;
};

// =======================================================

ClusterItem::ClusterItem()
        : Mass(0),
          SumX(0),
          SumY(0),
          Cx(0),
          Cy(0)
{ }

void ClusterItem::CalculateCenter()
{
    if (Mass > 0)
    {
        Cx = SumX / Mass;
        Cy = SumY / Mass;
    }
}

uint16_t Cluster::GetClustersAmount() const
{
    return m_ClustersAmount;
}
const ClusterItem &Cluster::GetCluster(uint16_t idx) const
{
    return (m_Clusters[idx]);
}

#ifndef max
#define max(a,b) ( (a) > (b) ? (a) : (b))
#define _CLUSTER_MAX_DEF_GUARD
#endif
#ifndef min
#define min(a,b) ( (a) < (b) ? (a) : (b))
#define _CLUSTER_MIN_DEF_GUARD
#endif

uint16_t Cluster::Clusterize(const BinaryImage &Src)
{
    const unsigned int MaxInt = MaxIdx;
    Create(Src.GetWidth(), Src.GetHeight(), PixelType::Mono16(MaxInt));

    m_ClustersAmount = 0;
    /*
    std::unique_ptr<uint16_t> upLookUpTable_Equiv(new uint16_t[MaxInt + 1]);
    std::unique_ptr<uint16_t> upLookUpTable_Idx(new uint16_t[MaxInt + 1]);
    uint16_t * LookUpTable_Equiv = upLookUpTable_Equiv.get();
    uint16_t * LookUpTable_Idx = upLookUpTable_Idx.get();
    */
    uint16_t LookUpTable_Equiv[MaxInt + 1];
    uint16_t LookUpTable_Idx[MaxInt + 1];
    memset(LookUpTable_Equiv, 0, sizeof(LookUpTable_Equiv));
    memset(LookUpTable_Idx, 0, sizeof(LookUpTable_Idx));
    /*
    for (uint16_t i = 0; i <= MaxInt; ++i)
    {
        LookUpTable_Equiv[i] = MaxInt;
        LookUpTable_Idx[i] = MaxInt;
    }*/

    uint16_t Clusters = 0;

    for (uint32_t y = 1; y < Src.GetHeight() - 1; ++y)
    {
        Cluster::iterator _ul = GetColRow(0, y - 1);
        Cluster::iterator _ur = GetColRow(2, y - 1);
        Cluster::iterator _u  = GetColRow(1, y - 1);
        Cluster::iterator _l  = GetColRow(0, y);
        Cluster::iterator *Neighbours[4] = {&_ul, &_u, &_ur, &_l};

        Cluster::iterator _c  = GetColRow(1, y);
        BinaryImage::iterator it = Src.GetColRow(1, y);

        for (uint32_t x = 1; x < Src.GetWidth() - 1; ++x, ++_ul, ++_u, ++_l, ++_c, ++_ur, ++it)
        {
            if (it[0] > 0)
            {
                uint16_t C = (*Neighbours[0])[0];
                for (uint8_t i = 1; i < 4; ++i)
                {
                    C = min(C, (*Neighbours[i])[0]);
                }

                for (uint8_t i = 0; i < 4; ++i)
                {
                    uint16_t _C = (*Neighbours[i])[0];
                    if (_C != C && _C != MaxInt)
                    {
                        LookUpTable_Equiv[_C] = C;
                    }
                }

                if (C < MaxInt)
                {
                    _c[0] = C;
                }
                else if (Clusters < MaxInt)
                {
                    _c[0] = Clusters;
                    LookUpTable_Equiv[Clusters] = Clusters;
                    Clusters++;
                }
            }
        }
    }

    Cluster::iterator it = begin();
    for (uint32_t y = 0; y < GetHeight(); ++y)
    {
        for (uint32_t x = 0; x < GetWidth(); ++x, ++it)
        {
            if (it[0] != MaxInt)
            {
                uint16_t Cluster = LookUpTable_Equiv[it[0]];
                uint16_t Idx = LookUpTable_Idx[Cluster];
                if (Idx != MaxInt)
                {
                    // Exists
                    m_Clusters[Idx].Mass++;
                    m_Clusters[Idx].SumX += x;
                    m_Clusters[Idx].SumY += y;
                }
                else
                {
                    // New
                    Idx = m_ClustersAmount;
                    LookUpTable_Idx[Cluster] = Idx;
                    m_Clusters[Idx].Mass = 1;
                    m_Clusters[Idx].SumX = x;
                    m_Clusters[Idx].SumY = y;
                    ++m_ClustersAmount;
                }
            }
        }
    }

    for (uint16_t i = 0; i < m_ClustersAmount; ++i)
    {
        m_Clusters[i].CalculateCenter();
    }
    return m_ClustersAmount;
}

#ifdef _CLUSTER_MAX_DEF_GUARD
#undef max
#endif
#ifdef _CLUSTER_MIN_DEF_GUARD
#undef min
#endif

#endif //JIMLIB_CLUSTER_HPP
