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

#ifndef JIMLIB_GRAYIMAGE_HPP
#define JIMLIB_GRAYIMAGE_HPP

#include "Image/GenericImage.hpp"
#include "Image/PixelTypes.hpp"
namespace jimlib
{
    class GrayImage : public GenericImage<PixelType::Mono8>
    {
    public:
        template<typename Pixel>
        void Convert(const GenericImage<Pixel> &RGB24Image);

        template<typename Pixel>
        void Convert(const GenericImage<Pixel> &Src, uint8_t Plant);

        // TODO: Move it to something like ImageProcessing class
        void AdjustColor(double k, double b);

        void CopyFrom(const GrayImage &Src);

        void CopyTo(GrayImage &Dst) const;
    };

// =======================================================

    template<typename Pixel>
    void GrayImage::Convert(const GenericImage<Pixel> &RGB24Image)
    {
        static_assert(CheckTypes<Pixel, PixelType::RGB24>::areSame || CheckTypes<Pixel, PixelType::RGBA32>::areSame,
                      "GrayImage.Convert allow only RGB24 or RGBA32 images");
        Create(RGB24Image.GetWidth(), RGB24Image.GetHeight());
        GenericImage<PixelType::RGB24>::const_iterator it_src = RGB24Image.begin();
        GrayImage::iterator it_dst = begin();
        for (; it_src != RGB24Image.end(); ++it_src, ++it_dst)
        {
            it_dst[0] = (6969 * it_src[0] + 23434 * it_src[1] + 2365 * it_src[2]) / 32768;
        }
    }

    template<typename Pixel>
    void GrayImage::Convert(const GenericImage<Pixel> &Src, uint8_t Plant)
    {
        assert(Plant < GenericImage<Pixel>::Plants);
        Create(Src.GetWidth(), Src.GetHeight());
        typename GenericImage<Pixel>::const_iterator it_src = Src.begin();
        GrayImage::iterator it_dst = begin();
        for (; it_src != Src.end(); ++it_src, ++it_dst)
        {
            it_dst[0] = it_src[Plant];
        }
    }

    inline void GrayImage::CopyFrom(const GrayImage &Src)
    {
        CopyFromInternal(Src);
    }

    inline void GrayImage::CopyTo(GrayImage &Dst) const
    {
        CopyToInternal(Dst);
    }

    inline void GrayImage::AdjustColor(double k, double b)
    {
        GrayImage::iterator it = begin();
        for (; it != end(); ++it)
        {
            int32_t V = (k * it[0] + b);
            V = V < 0 ? 0 : V;
            V = V > 255 ? 255 : V;
            it[0] = V;
        }
    }
};
#endif //JIMLIB_GRAYIMAGE_HPP
