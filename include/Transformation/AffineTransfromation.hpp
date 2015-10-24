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

#ifndef JIMLIB_AFFINETRANSFROM_HPP
#define JIMLIB_AFFINETRANSFROM_HPP

#include "Transformation/GenericTransformationTable.hpp"

namespace jimlib
{
    class AffineTransformation : public GenericTransformationTable
    {
    public:
        void Calculate(const uint32_t Width, const uint32_t Height, const double *AffineCoeffs, bool Resize);
    private:
        void CalculateInverse(const double *AffineCoeffs);
        double m_InverseAffine[6];
    };

    inline void AffineTransformation::Calculate(const uint32_t Width, const uint32_t Height, const double *AffineCoeffs, bool Resize)
    {
        assert(Resize == false);
        CalculateInverse(AffineCoeffs);
        Create(Width, Height, CoordsXY16(0,0));
        iterator it = begin();
        for (uint32_t y = 0; y < Height; ++y)
        {
            for (uint32_t x = 0; x < Width; ++x, ++it)
            {
                int _x = (uint32_t)(m_InverseAffine[0] * x + m_InverseAffine[1] * y + m_InverseAffine[2] + 0.5);
                int _y = (uint32_t)(m_InverseAffine[3] * x + m_InverseAffine[4] * y + m_InverseAffine[5] + 0.5);
                if (_x > 0 && _y > 0 && _x < Width && _y < Height)
                {
                    it[0] = _x;
                    it[1] = _y;
                }
            }
        }
    }
    inline void AffineTransformation::CalculateInverse(const double *AffineCoeffs)
    {
        double a = AffineCoeffs[0];
        double b = AffineCoeffs[1];
        double c = AffineCoeffs[2];
        double d = AffineCoeffs[3];
        double e = AffineCoeffs[4];
        double f = AffineCoeffs[5];

        double divisor = a * e - b * d;

        if (divisor > -1.0e-8 && divisor < 1.0e-8)
        {
            m_InverseAffine[0] = 0;
            m_InverseAffine[1] = 0;
            m_InverseAffine[2] = 0;
            m_InverseAffine[3] = 0;
            m_InverseAffine[4] = 0;
            m_InverseAffine[5] = 0;
            return;
        }
        m_InverseAffine[0] = e / divisor;
        m_InverseAffine[1] = -b / divisor;
        m_InverseAffine[2] = (b * f - e * c) / divisor;
        m_InverseAffine[3] = -d / divisor;
        m_InverseAffine[4] = a / divisor;
        m_InverseAffine[5] = (c * d - a * f) / divisor;
    }
}

#endif //JIMLIB_AFFINETRANSFROM_HPP
