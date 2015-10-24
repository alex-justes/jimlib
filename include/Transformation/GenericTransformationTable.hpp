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

#ifndef JIMLIB_GENERICTRANSFORMATIONTABLE_HPP
#define JIMLIB_GENERICTRANSFORMATIONTABLE_HPP

#include "Image/GenericImage.hpp"

namespace jimlib
{
    class CoordsXY16 : public GenericPixel<uint32_t, 2>
    {
    public:
        CoordsXY16();
        CoordsXY16(uint32_t X, uint32_t Y);
        CoordsXY16(const CoordsXY16 &Coords);
        uint32_t &X;
        uint32_t &Y;
    };

    namespace InterpolationType
    {
        const uint32_t NearestNeighbour = 0;
    };

    class GenericTransformationTable : public GenericImage<CoordsXY16>
    {
    public:
        template<uint32_t Interpolation, typename Pixel>
        void Apply(const GenericImage<Pixel> &Src, GenericImage<Pixel> &Dst) const;
    };

    inline CoordsXY16::CoordsXY16()
            : X(m_Buffer[0]),
              Y(m_Buffer[1])
    {
    }

    inline CoordsXY16::CoordsXY16(uint32_t _X, uint32_t _Y)
            : X(m_Buffer[0]),
              Y(m_Buffer[1])
    {
        X = _X;
        Y = _Y;
    }

    inline CoordsXY16::CoordsXY16(const CoordsXY16 &Coords)
            : X(m_Buffer[0]),
              Y(m_Buffer[1])
    {
        X = Coords.X;
        Y = Coords.Y;
    }

    template<uint32_t Interpolation, typename Pixel>
    void GenericTransformationTable::Apply(const GenericImage<Pixel> &Src, GenericImage<Pixel> &Dst) const
    {
        static_assert(Interpolation == InterpolationType::NearestNeighbour,
        "Usupported Interpolation mode!");

        Dst.Create(GetWidth(), GetHeight());

        uint32_t sx = 0;
        uint32_t sy = 0;
        uint32_t ex = Dst.GetWidth();
        uint32_t ey = Dst.GetHeight();

        for (unsigned int y = sy; y < ey; ++y)
        {
            typename GenericImage<Pixel>::iterator it_dst = Dst.GetColRow(sx, y);
            GenericTransformationTable::const_iterator it_src = GetColRow(sx, y);
            for (unsigned int x = sx; x < ex; ++x, ++it_dst, ++it_src)
            {
                for (unsigned int p = 0; p < GenericImage<Pixel>::Plants; ++p)
                {
                    switch(Interpolation)
                    {
                        case InterpolationType::NearestNeighbour :
                            {
                                it_dst[p] = Src.GetPixel(it_src[0], it_src[1], p);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    };
};

#endif //JIMLIB_GENERICTRANSFORMATIONTABLE_HPP
