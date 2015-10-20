#include "Image/GrayImage.hpp"
#include "Image/BinaryImage.hpp"
#include "Processing/FastGaussianBlur.hpp"
#include "Processing/Cluster.hpp"
#include "PngImage.h"

/*!
 *  \file
 *  \brief Simple example
 *  \author Alexey Titov
 *  \date October 2015
 *
 *  Here is a little example of jimlib usage.
 *  Dependencies: libpng
 */


using namespace jimlib;

int main()
{
    PngImage Png;
    Png.Read("./cballs.png");

    GrayImage GrayOriginal, GrayBlured, GrayAdjusted;
    // Convert original PNG to gray-level image
    GrayOriginal.Convert(Png);

    // Blur PNG image
    FastGaussianBlur::Blur<3>(Png, 4.0);
    Png.Write("./cballs_blured.png");

    // Convert Blured PNG image to gray-level image
    GrayBlured.Convert(Png);
    GrayAdjusted.CopyFrom(GrayBlured);

    // Adjust color
    GrayAdjusted.AdjustColor(1.5, -250);

    Png.Write(GrayOriginal, "./cballs_gray.png");
    Png.Write(GrayBlured, "./cballs_grayblured.png");
    Png.Write(GrayAdjusted, "./cballs_grayadjusted.png");

    BinaryImage Bin;

    // Example of Otsu binarization
    Bin.Otsu(GrayAdjusted);
    Png.Write(Bin, "./cballs_otsu.png");

    // Example of Niblack binarization
    Bin.Niblack(GrayAdjusted, 150, 1.5);
    Png.Write(Bin, "./cballs_niblack.png");

    Png.Read("./cballs.png");
    Cluster Objects;
    // Clusterize white objects (glowing white balls)
    uint16_t ClusterAmount = Objects.Clusterize(Bin);
    for (int i = 0; i < ClusterAmount; ++i)
    {
        const ClusterItem &item = Objects.GetCluster(i);
        Png.DrawCross(item.Cx, item.Cy, 20, PixelType::RGB24(0,255,0));
    }
    Png.Write("./cballs_clusterized.png");
    return 0;
}