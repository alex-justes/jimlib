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

#ifndef JIMLIB_CLUSTER_HPP
#define JIMLIB_CLUSTER_HPP

#include <memory>
#include <list>
#include "Image/PixelTypes.hpp"
#include "Image/BinaryImage.hpp"
#include "Utils/MinMax.hpp"
#include "Utils/Graph.hpp"

namespace jimlib
{
    class Cluster;

    class ClusterItem
    {
    friend class Cluster;
    public:
        ClusterItem();
        uint16_t Id;
        uint32_t Mass;
        uint64_t SumX;
        uint64_t SumY;
        uint32_t Cx;
        uint32_t Cy;
        double fCx;
        double fCy;

        void CalculateCenter();
    private:
        bool m_Used;
        uint32_t m_Parent;
    };

    class Cluster : public GenericImage<PixelType::Mono16>
    {
    public:
        static const uint32_t MaxIdx = UINT16_MAX;
        static const uint32_t MaxClusters = UINT16_MAX;

        template<typename Pixel>
        uint16_t ClusterizeMask(const GenericImage<Pixel> &Image, const BinaryImage &Mask);

        uint16_t Clusterize(const BinaryImage &Image);

        uint16_t GetClustersAmount() const;

        uint16_t MergeNearbyClusters(double Distance);

        const ClusterItem &GetCluster(uint16_t idx) const;

    private:
        struct NetWalkData
        {
            uint16_t Parent;
            ClusterItem * pClusters;
        };
        void ClusterizeInternal(const BinaryImage &Image);

        template<typename Pixel>
        uint16_t ExtractClustersInternal(const GenericImage<Pixel> &Image);

        ClusterItem m_Clusters[UINT16_MAX];
        uint16_t m_ClustersAmount;
        uint16_t m_LookUpTable_Equiv[MaxIdx + 1];
        uint16_t m_LookUpTable_Idx[MaxIdx + 1];
    };

// =======================================================

    template<typename Pixel>
    uint16_t Cluster::ClusterizeMask(const GenericImage<Pixel> &Image, const BinaryImage &Mask)
    {
        static_assert(GenericImage<Pixel>::Plants == 1, "Only Images with 1 plant are allowed!");
        ClusterizeInternal(Mask);
        return ExtractClustersInternal(Image);
    }

    template<typename Pixel>
    uint16_t Cluster::ExtractClustersInternal(const GenericImage<Pixel> &Image)
    {
        static_assert(GenericImage<Pixel>::Plants == 1, "Only Images with 1 plant are allowed!");
        typename GenericImage<Pixel>::const_iterator it_mass = Image.begin();
        Cluster::iterator it = begin();
        for (uint32_t y = 0; y < GetHeight(); ++y)
        {
            for (uint32_t x = 0; x < GetWidth(); ++x, ++it, ++it_mass)
            {
                if (it[0] != MaxIdx)
                {
                    uint16_t Cluster = m_LookUpTable_Equiv[it[0]];
                    uint16_t Idx = m_LookUpTable_Idx[Cluster];
                    if (Idx != MaxIdx)
                    {
                        // Exists
                        uint8_t p = it_mass[0];
                        m_Clusters[Idx].Mass += p;
                        m_Clusters[Idx].SumX += p * x;
                        m_Clusters[Idx].SumY += p * y;
                    }
                    else
                    {
                        // New
                        uint8_t p = it_mass[0];
                        Idx = m_ClustersAmount;
                        m_LookUpTable_Idx[Cluster] = Idx;
                        m_Clusters[Idx].Mass = p;
                        m_Clusters[Idx].SumX = p * x;
                        m_Clusters[Idx].SumY = p * y;
                        m_Clusters[Idx].Id = Idx;
                        ++m_ClustersAmount;
                    }
                    it[0] = Idx;
                }
            }
        }
        for (uint16_t i = 0; i < m_ClustersAmount; ++i)
        {
            m_Clusters[i].CalculateCenter();
        }
        return m_ClustersAmount;
    }

    inline ClusterItem::ClusterItem()
            : Mass(0),
              SumX(0),
              SumY(0),
              Cx(0),
              Cy(0)
    { }

    inline void ClusterItem::CalculateCenter()
    {
        if (Mass > 0)
        {
            Cx = SumX / Mass;
            Cy = SumY / Mass;
            fCx = (double)SumX / Mass;
            fCy = (double)SumY / Mass;
        }
    }

    inline uint16_t Cluster::Clusterize(const BinaryImage &Image)
    {
        ClusterizeInternal(Image);
        return ExtractClustersInternal(Image);
    }
    inline uint16_t Cluster::GetClustersAmount() const
    {
        return m_ClustersAmount;
    }

    inline const ClusterItem &Cluster::GetCluster(uint16_t idx) const
    {
        return (m_Clusters[idx]);
    }
    inline void Cluster::ClusterizeInternal(const BinaryImage &Image)
    {
        const unsigned int MaxInt = MaxIdx;
        Create(Image.GetWidth(), Image.GetHeight(), PixelType::Mono16(MaxInt));
        m_ClustersAmount = 0;
        for (unsigned int i = 0; i <= MaxInt; ++i)
        {
            m_LookUpTable_Equiv[i] = MaxInt;
            m_LookUpTable_Idx[i] = MaxInt;
        }
        uint16_t Clusters = 0;
        for (uint32_t y = 1; y < Image.GetHeight() - 1; ++y)
        {
            Cluster::iterator _ul = GetColRow(0, y - 1);
            Cluster::iterator _ur = GetColRow(2, y - 1);
            Cluster::iterator _u = GetColRow(1, y - 1);
            Cluster::iterator _l = GetColRow(0, y);
            Cluster::iterator *Neighbours[4] = {&_ul, &_u, &_ur, &_l};
            Cluster::iterator _c = GetColRow(1, y);
            BinaryImage::const_iterator it = Image.GetColRow(1, y);
            for (uint32_t x = 1; x < Image.GetWidth() - 1; ++x, ++_ul, ++_u, ++_l, ++_c, ++_ur, ++it)
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
                            while (m_LookUpTable_Equiv[C] != C)
                            {
                                C = m_LookUpTable_Equiv[C];
                            }
                            m_LookUpTable_Equiv[_C] = C;
                        }
                    }
                    if (C < MaxInt)
                    {
                        _c[0] = C;
                    }
                    else if (Clusters < MaxInt)
                    {
                        _c[0] = Clusters;
                        m_LookUpTable_Equiv[Clusters] = Clusters;
                        Clusters++;
                    }
                }
            }
        }
    }
    inline uint16_t Cluster::MergeNearbyClusters(double Distance)
    {
        for (uint32_t i = 0; i < m_ClustersAmount; ++i)
        {
            m_Clusters[i].m_Used = false;
        }
        std::list<std::pair<uint32_t,uint32_t>> pairs;
        for (uint32_t i = 0; i < m_ClustersAmount; ++i)
        {
            if (!m_Clusters[i].m_Used)
            {
                for (uint32_t j = i + 1; j < m_ClustersAmount; ++j)
                {
                    double Dy = m_Clusters[i].fCy - m_Clusters[j].fCy;
                    double Dx = m_Clusters[i].fCx - m_Clusters[j].fCx;
                    double D = sqrt(Dy*Dy + Dx*Dx);
                    if (D < Distance)
                    {
                        pairs.push_back(std::make_pair(i,j));
                    }
                }
            }
        }
        
        for (const auto &p : pairs)
        {
            // Always merge with first
            bool usedBoth = m_Clusters[p.first].m_Used && m_Clusters[p.second].m_Used;
            if (usedBoth)
            {
                continue;
            }
            uint32_t parent = p.first;
            uint32_t node   = p.second;
            bool usedAny = m_Clusters[p.first].m_Used || m_Clusters[p.second].m_Used;
            if (usedAny)
            {
                parent = m_Clusters[p.first].m_Used ? m_Clusters[p.first].m_Parent : m_Clusters[p.second].m_Parent;
                node   = m_Clusters[p.first].m_Used ? p.second : p.first;
            }
            m_Clusters[parent].SumX += m_Clusters[node].SumX;
            m_Clusters[parent].SumY += m_Clusters[node].SumY;
            m_Clusters[parent].Mass += m_Clusters[node].Mass;
            m_Clusters[p.first].m_Used    = true;
            m_Clusters[p.first].m_Parent  = parent;
            m_Clusters[p.second].m_Used   = true;
            m_Clusters[p.second].m_Parent = parent;
        }
        for (uint32_t idx = 0; idx < m_ClustersAmount; ++idx)
        {
            m_Clusters[m_Clusters[idx].m_Parent].m_Used = false;
        }
        uint16_t RealClusters = 0;
        for (uint32_t i = 0; i < m_ClustersAmount; ++i)
        {
            if (!m_Clusters[i].m_Used)
            {
                m_Clusters[RealClusters].SumX = m_Clusters[i].SumX;
                m_Clusters[RealClusters].SumY = m_Clusters[i].SumY;
                m_Clusters[RealClusters].Mass = m_Clusters[i].Mass;
                m_Clusters[RealClusters].CalculateCenter();
                ++RealClusters;
            }
        }
        m_ClustersAmount = RealClusters;
        return m_ClustersAmount;
    }
};

#endif //JIMLIB_CLUSTER_HPP
