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

#ifndef CLUSTER_H
#define CLUSTER_H

#include "GenericImage.h"
#include "PixelTypes.h"
#include "BinaryImage.h"
#include <cstdint>
#include <memory>

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

#endif //CLUSTER_H
