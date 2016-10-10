//
// Created by justes on 20.10.15.
//

#ifndef JIMLIB_PNGIMAGE_H
#define JIMLIB_PNGIMAGE_H

#include <string>
#include <png.h>
#include "Image/GenericImage.hpp"
#include "Image/PixelTypes.hpp"
#include "Image/BinaryImage.hpp"
#include "Image/GrayImage.hpp"

class PngImage : public jimlib::GenericImage<jimlib::PixelType::RGB24>
{
public:
    PngImage();
    virtual ~PngImage();
    bool Read(const std::string & FileName);
    bool Write(const std::string & FileName) const;
    bool Write(const jimlib::BinaryImage &Src, const std::string & FileName);
    bool Write(const jimlib::GrayImage &Src, const std::string & FileName);
    void DrawCross(unsigned int Cx, unsigned int Cy, unsigned int Rad, const jimlib::PixelType::RGB24 &Value);

    void Convert(const jimlib::BinaryImage &Src);
    void Convert(const jimlib::GrayImage &Src);
private:
    void FreeImageMemory();
    bool AllocImageMemory(unsigned int Width, unsigned int Height, const png_structp pPng, const png_infop pInfo);
    png_byte m_ColorType;
    png_byte m_BitDepth;
    int m_Passes;
    unsigned int m_RowSize;
    png_bytep * m_pRows;
};

#endif //JIMLIB_PNGIMAGE_H
