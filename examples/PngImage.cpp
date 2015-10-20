//
// Created by justes on 20.10.15.
//
#include "PngImage.h"
#include "Utils/MinMax.hpp"

using namespace jimlib;

PngImage::PngImage()
        : GenericImage(),
          m_Passes(0),
          m_RowSize(0),
          m_pRows(nullptr)
{

}
PngImage::~PngImage()
{
    if (m_pRows)
    {
        delete[] m_pRows;
    }
}
void PngImage::FreeImageMemory()
{
    if (m_pRows)
    {
        delete[] m_pRows;
        m_pRows = nullptr;
    }
}
bool PngImage::AllocImageMemory(unsigned int Width, unsigned int Height, const png_structp pPng, const png_infop pInfo)
{
    unsigned int newRowSize = png_get_rowbytes(pPng, pInfo);
    if (m_RowSize != newRowSize || m_Height != Height || Width != m_Width)
    {
        FreeImageMemory();
        m_RowSize = newRowSize;
        if (newRowSize / Width != 3)
        {
            return false;
        }
        Create(Width, Height);
        m_pRows = new png_bytep[Height];
        for (unsigned int y = 0; y < m_Height; ++y)
        {
            m_pRows[y] = reinterpret_cast<png_byte *>(m_RawData + y * m_RowSize);
        }
    }
    return true;
}


bool PngImage::Read(const std::string &FileName)
{
    png_structp pPng;
    png_infop pInfo;
    unsigned char Hdr[8];
    FILE *fp = fopen(FileName.c_str(), "rb");
    if (!fp)
    {
        return false;
    }
    fread(Hdr, 1, 8, fp);
    if (png_sig_cmp(Hdr, 0, 8))
    {
        return false;
    }

    pPng = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pPng)
    {
        return false;
    }
    pInfo = png_create_info_struct(pPng);
    if (!pInfo)
    {
        return false;
    }
    if (setjmp(png_jmpbuf(pPng)))
    {
        return false;
    }
    png_init_io(pPng, fp);
    png_set_sig_bytes(pPng, 8);
    png_read_info(pPng, pInfo);

    unsigned int Width = png_get_image_width(pPng, pInfo);
    unsigned int Height = png_get_image_height(pPng, pInfo);

    m_ColorType = png_get_color_type(pPng, pInfo);
    m_BitDepth = png_get_bit_depth(pPng, pInfo);

    m_Passes = png_set_interlace_handling(pPng);

    png_read_update_info(pPng, pInfo);

    if (setjmp(png_jmpbuf(pPng)))
    {
        return false;
    }

    if (!AllocImageMemory(Width, Height, pPng, pInfo))
    {
        return false;
    }

    png_read_image(pPng, m_pRows);
    fclose(fp);
    return true;
}

bool PngImage::Write(const std::string &FileName) const
{
    FILE *fp = fopen(FileName.c_str(), "wb");
    if (!fp)
    {
        return false;
    }
    png_structp pPng = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pPng)
    {
        return false;
    }
    png_infop pInfo = png_create_info_struct(pPng);
    if (!pInfo)
    {
        return false;
    }
    if (setjmp(png_jmpbuf(pPng)))
    {
        return false;
    }
    png_init_io(pPng, fp);
    if (setjmp(png_jmpbuf(pPng)))
    {
        return false;
    }

    png_set_IHDR(pPng, pInfo, m_Width, m_Height,
                 m_BitDepth, m_ColorType, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(pPng, pInfo);

    if (setjmp(png_jmpbuf(pPng)))
    {
        return false;
    }

    png_write_image(pPng, m_pRows);
    if (setjmp(png_jmpbuf(pPng)))
    {
        return false;
    }
    png_write_end(pPng, NULL);
    fclose(fp);
    return true;
}

void PngImage::Convert(const jimlib::BinaryImage &Src)
{
    BinaryImage::const_iterator it_src = Src.begin();
    iterator it_dst = begin();

    for (; it_src != Src.end(); ++it_src, ++it_dst)
    {
        it_dst[0] = it_src[0] * 255;
        it_dst[1] = it_src[0] * 255;
        it_dst[2] = it_src[0] * 255;
    }
}

void PngImage::Convert(const jimlib::GrayImage &Src)
{
    GrayImage::const_iterator it_src = Src.begin();
    iterator it_dst = begin();

    for (; it_src != Src.end(); ++it_src, ++it_dst)
    {
        it_dst[0] = it_src[0];
        it_dst[1] = it_src[0];
        it_dst[2] = it_src[0];
    }
}

bool PngImage::Write(const jimlib::BinaryImage &Src, const std::string & FileName)
{
    Convert(Src);
    return Write(FileName);
}

bool PngImage::Write(const jimlib::GrayImage &Src, const std::string & FileName)
{
    Convert(Src);
    return Write(FileName);
}

void PngImage::DrawCross(unsigned int Cx, unsigned int Cy, unsigned int Rad, const jimlib::PixelType::RGB24 &Value)
{
    int sx = min(max((unsigned int)0, Cx - Rad), GetWidth() - 1);
    int ex = min(max((unsigned int)0, Cx + Rad), GetWidth() - 1);

    for (int x = sx; x <= ex; ++x)
    {
        SetPixel(x, Cy, Value);
    }

    int sy = min(max((unsigned int)0, Cy - Rad), GetHeight() - 1);
    int ey = min(max((unsigned int)0, Cy + Rad), GetHeight() - 1);
    for (int y = sy; y <= ey; ++y)
    {
        SetPixel(Cx, y, Value);
    }
}