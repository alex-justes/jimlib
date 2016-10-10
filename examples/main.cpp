#include "Image/GrayImage.hpp"
#include "Image/BinaryImage.hpp"
#include "Processing/FastGaussianBlur.hpp"
#include "Processing/Cluster.hpp"
#include "Transformation/AffineTransfromation.hpp"
#include "PngImage.h"

#include "Convolution/Convolution.hpp"
#include "EdgeDetection/Sobel.hpp"
#include "Utils/Search.hpp"
/*!
 *  \file
 *  \brief Simple example
 *  \author Alexey Titov
 *  \date October 2015
 *
 *  Here is a little example of jimlib usage.
 *  You should put cballs.png in current working directory in order it to process it correctly.
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

    // Example of Sauvola binarization
    Bin.Sauvola(GrayAdjusted, 20, 0.2);
    Png.Write(Bin, "./cballs_sauvola.png");

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


    Png.Read("./cballs.png");
    PngImage Png2;
    Png2.Read("./cballs.png");
    AffineTransformation Affine;
    AffineTransformation Affine2;

    GrayImage Gray1, Gray2;
    Gray1.Convert(Png);

    //Affine.Shift(20, 50);
    Affine.RotateDeg(22.5);
    Affine2.Scale(2, 2);
    //Affine.Scale(0.5, 0.5);

    Affine.Transform(Affine2);

    AffineTransformationTable AffineTable;
    AffineTable.Calculate(Gray1.GetWidth(), Gray1.GetHeight(), Affine, true);
    AffineTable.Apply<InterpolationType::NearestNeighbour>(Gray1, Gray2);
    Png.Write(Gray2, "./cballs_affine.png");
    
    PngImage testPng;
    testPng.Read("./cballs.png");
    GrayImage testGray;
    testGray.Convert(testPng);
    
    Sobel<int32_t, int32_t> sobel;

    sobel.Calculate(testGray);
    const GenericImage<GenericPixel<int32_t , 1>> *magn = sobel.GetMagnitude();
    
    int32_t min_ = min(*magn);
    int32_t max_ = max(*magn);
    int32_t factor = max_ - min_;
    printf("%d %d %d\n", min_, max_, factor);
    auto it_src = magn->begin();
    auto it_dst = testGray.begin();
    for (; it_src != magn->end(); ++it_src, ++it_dst)
    {
        if (factor > 0)
        {
            int32_t p = it_src[0];
            p -= min_;
            it_dst[0] = (unsigned char) min(255, max(0, (255 * p) / factor));
        }
        else
        {
            it_dst[0] = (unsigned char) min(255, max(0, it_src[0]));
        }
    }
    testPng.Write(testGray, "./cballs_sobel.png");
    
    return 0;
}