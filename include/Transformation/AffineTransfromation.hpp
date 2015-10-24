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

#ifndef JIMLIB_AFFINETRANSFORMATIONTABLE_HPP
#define JIMLIB_AFFINETRANSFORMATIONTABLE_HPP

#include <cassert>
#include <cmath>
#include "Transformation/GenericTransformationTable.hpp"
#include "Utils/Point.hpp"
#include "Utils/MinMax.hpp"

namespace jimlib
{
    class AffineTransformation
    {
    public:
        AffineTransformation();
        AffineTransformation(const AffineTransformation &Transformation);
        AffineTransformation(const double *AffineCoeffs);
        double &operator[](uint8_t i);
        const double &operator[](uint8_t i) const;

        Point Transform(const Point &Pt) const;

        void Transform(const AffineTransformation &Transformation);
        void RotateRad(double Angle);
        void RotateDeg(double Angle);
        void Shift(double ShiftX, double ShiftY);
        void Scale(double ScaleX, double ScaleY);
        void Inverse();
    private:
        double m_Affine[6];
    };

    class AffineTransformationTable : public GenericTransformationTable
    {
    public:
        void Calculate(uint32_t Width, uint32_t Height, const AffineTransformation &Affine, bool Autofit);
    };

    // =======================================================

    // No transformation
    inline AffineTransformation::AffineTransformation()
    {
        m_Affine[0] = 1;
        m_Affine[1] = 0;
        m_Affine[2] = 0;
        m_Affine[3] = 0;
        m_Affine[4] = 1;
        m_Affine[5] = 0;
    }
    inline AffineTransformation::AffineTransformation(const AffineTransformation &Transformation)
    {
        memcpy(m_Affine, Transformation.m_Affine, sizeof(m_Affine));
    }
    inline AffineTransformation::AffineTransformation(const double *AffineCoeffs)
    {
        memcpy(m_Affine, AffineCoeffs, sizeof(m_Affine));
    }
    inline double &AffineTransformation::operator[](uint8_t i)
    {
        assert(i < 6);
        return m_Affine[i];
    }
    inline const double &AffineTransformation::operator[](uint8_t i) const
    {
        assert(i < 6);
        return m_Affine[i];
    }

    inline Point AffineTransformation::Transform(const Point &Pt) const
    {
        Point _Pt;
        _Pt.x = Pt.x * m_Affine[0] + Pt.y * m_Affine[1] + m_Affine[2];
        _Pt.y = Pt.x * m_Affine[3] + Pt.y * m_Affine[4] + m_Affine[5];
        return _Pt;
    }

    inline void AffineTransformation::Transform(const AffineTransformation &Transformation)
    {
        double a = m_Affine[0];
        double b = m_Affine[1];
        double c = m_Affine[2];
        double d = m_Affine[3];
        double e = m_Affine[4];
        double f = m_Affine[5];
        double _a = Transformation[0];
        double _b = Transformation[1];
        double _c = Transformation[2];
        double _d = Transformation[3];
        double _e = Transformation[4];
        double _f = Transformation[5];

        m_Affine[0] = (a*_a + b*_d);
        m_Affine[1] = (a*_b + b*_e);
        m_Affine[2] = (a*_c + b*_f + c);
        m_Affine[3] = (d*_a + e*_d);
        m_Affine[4] = (d*_b + e*_e);
        m_Affine[5] = (d*_c + e*_f + f);
    }

    inline void AffineTransformation::RotateRad(double Angle)
    {
        double a = m_Affine[0];
        double b = m_Affine[1];
        double d = m_Affine[3];
        double e = m_Affine[4];

        m_Affine[0] =  cos(Angle) * a + sin(Angle) * b;
        m_Affine[1] = -sin(Angle) * a + cos(Angle) * b;
        m_Affine[3] =  cos(Angle) * d + sin(Angle) * e;
        m_Affine[4] = -sin(Angle) * d + cos(Angle) * e;
    }
    inline void AffineTransformation::RotateDeg(double Angle)
    {
        Angle = M_PI * Angle / 180;
        RotateRad(Angle);
    }

    inline void AffineTransformation::Scale(double ScaleX, double ScaleY)
    {
        m_Affine[0] = m_Affine[0] * ScaleX;
        m_Affine[1] = m_Affine[1] * ScaleY;
        m_Affine[3] = m_Affine[3] * ScaleX;
        m_Affine[4] = m_Affine[4] * ScaleY;
    }

    inline void AffineTransformation::Shift(double ShiftX, double ShiftY)
    {
        m_Affine[2] += ShiftX;
        m_Affine[5] += ShiftY;
    }

    inline void AffineTransformation::Inverse()
    {
        double a = m_Affine[0];
        double b = m_Affine[1];
        double c = m_Affine[2];
        double d = m_Affine[3];
        double e = m_Affine[4];
        double f = m_Affine[5];

        double divisor = a * e - b * d;

        if (divisor > -1.0e-8 && divisor < 1.0e-8)
        {
            m_Affine[0] = 1;
            m_Affine[1] = 0;
            m_Affine[2] = 0;
            m_Affine[3] = 0;
            m_Affine[4] = 1;
            m_Affine[5] = 0;
            return;
        }
        m_Affine[0] = e / divisor;
        m_Affine[1] = -b / divisor;
        m_Affine[2] = (b * f - e * c) / divisor;
        m_Affine[3] = -d / divisor;
        m_Affine[4] = a / divisor;
        m_Affine[5] = (c * d - a * f) / divisor;
    }

    inline void AffineTransformationTable::Calculate(uint32_t Width, uint32_t Height, const AffineTransformation &Affine, bool Autofit)
    {
        AffineTransformation InverseAffine = Affine;
        if (Autofit)
        {
            Point p1 = Affine.Transform(Point(0,0));
            Point p2 = Affine.Transform(Point(Width,0));
            Point p3 = Affine.Transform(Point(0,Height));
            Point p4 = Affine.Transform(Point(Width,Height));

            double MinX = p1.x;
            double MinY = p1.y;
            double MaxX = p1.x;
            double MaxY = p1.y;

            MinX = min(MinX, (double)p2.x);
            MinX = min(MinX, (double)p3.x);
            MinX = min(MinX, (double)p4.x);
            MinY = min(MinY, (double)p2.y);
            MinY = min(MinY, (double)p3.y);
            MinY = min(MinY, (double)p4.y);

            MaxX = max(MaxX, (double)p2.x);
            MaxX = max(MaxX, (double)p3.x);
            MaxX = max(MaxX, (double)p4.x);
            MaxY = max(MaxY, (double)p2.y);
            MaxY = max(MaxY, (double)p3.y);
            MaxY = max(MaxY, (double)p4.y);

            InverseAffine.Shift(-MinX, -MinY);
            Width = (uint32_t)abs((int32_t)(MaxX - MinX + 0.5));
            Height = (uint32_t)abs((int32_t)(MaxY - MinY + 0.5));
        }

        Create(Width, Height, CoordsXY16(0,0));

        InverseAffine.Inverse();
        iterator it = begin();
        for (uint32_t y = 0; y < Height; ++y)
        {
            for (uint32_t x = 0; x < Width; ++x, ++it)
            {
                int _x = (uint32_t)(InverseAffine[0] * x + InverseAffine[1] * y + InverseAffine[2] + 0.5);
                int _y = (uint32_t)(InverseAffine[3] * x + InverseAffine[4] * y + InverseAffine[5] + 0.5);
                if (_x > 0 && _y > 0 && _x < Width && _y < Height)
                {
                    it[0] = _x;
                    it[1] = _y;
                }
            }
        }
    }
};

#endif //JIMLIB_AFFINETRANSFORMATIONTABLE_HPP
