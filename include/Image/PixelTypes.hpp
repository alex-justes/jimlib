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

#ifndef JIMLIB_PIXELTYPES_H
#define JIMLIB_PIXELTYPES_H

#include "Image/GenericPixel.hpp"

namespace PixelMode
{
    const uint8_t None   = 0;
    const uint8_t Mono8  = 1;
    const uint8_t Gray   = Mono8;
    const uint8_t Mono16 = 2;
    const uint8_t Mono32 = 3;
    const uint8_t Mono64 = 4;
    const uint8_t RGB24  = 5;
    const uint8_t RGBA32 = 6;
}
namespace PixelType
{
    class Mono8 : public GenericPixel<uint8_t, 1>
    {
    public:
        Mono8();
        Mono8(uint8_t Value);
        Mono8(const Mono8 &Pixel);
        uint8_t &Value;
    };

    class Mono16 : public GenericPixel<uint16_t, 1>
    {
    public:
        Mono16();
        Mono16(uint16_t Value);
        Mono16(const Mono16 &Pixel);
        uint16_t &Value;
    };

    class Mono32 : public GenericPixel<uint32_t, 1>
    {
    public:
        Mono32();
        Mono32(uint32_t Value);
        Mono32(const Mono32 &Pixel);
        uint32_t &Value;
    };

    class Mono64 : public GenericPixel<uint64_t, 1>
    {
    public:
        Mono64();
        Mono64(uint64_t Value);
        Mono64(const Mono64 &Pixel);
        uint64_t &Value;
    };

    class RGB24 : public GenericPixel<uint8_t, 3>
    {
    public:
        RGB24();
        RGB24(uint8_t R, uint8_t G, uint8_t B);
        RGB24(const RGB24 &Pixel);
        uint8_t &R;
        uint8_t &G;
        uint8_t &B;
    };

    class RGBA32 : public GenericPixel<uint8_t, 4>
    {
    public:
        RGBA32();
        RGBA32(uint8_t R, uint8_t G, uint8_t B, uint8_t A);
        RGBA32(const RGBA32 &Pixel);
        uint8_t &R;
        uint8_t &G;
        uint8_t &B;
        uint8_t &A;
    };
}

// =======================================================

using namespace PixelType;

Mono8::Mono8()
        : Value(m_Buffer[0])
{
    Value = 0;
}

Mono8::Mono8(uint8_t _Value)
        : Value(m_Buffer[0])
{
    Value = _Value;
}

Mono8::Mono8(const Mono8 &Pixel)
        : Value(m_Buffer[0])
{
    Value = Pixel.Value;
}

Mono16::Mono16()
        : Value(m_Buffer[0])
{
    Value = 0;
}

Mono16::Mono16(uint16_t _Value)
        : Value(m_Buffer[0])
{
    Value = _Value;
}

Mono16::Mono16(const Mono16 &Pixel)
        : Value(m_Buffer[0])
{
    Value = Pixel.Value;
}

Mono32::Mono32()
        : Value(m_Buffer[0])
{
    Value = 0;
}

Mono32::Mono32(uint32_t _Value)
        : Value(m_Buffer[0])
{
    Value = _Value;
}

Mono32::Mono32(const Mono32 &Pixel)
        : Value(m_Buffer[0])
{
    Value = Pixel.Value;
}

Mono64::Mono64()
        : Value(m_Buffer[0])
{
    Value = 0;
}

Mono64::Mono64(uint64_t _Value)
        : Value(m_Buffer[0])
{
    Value = _Value;
}

Mono64::Mono64(const Mono64 &Pixel)
        : Value(m_Buffer[0])
{
    Value = Pixel.Value;
}

RGB24::RGB24()
        : R(m_Buffer[0]),
          G(m_Buffer[1]),
          B(m_Buffer[2])
{
    R = 0;
    G = 0;
    B = 0;
}

RGB24::RGB24(uint8_t _R, uint8_t _G, uint8_t _B)
        : R(m_Buffer[0]),
          G(m_Buffer[1]),
          B(m_Buffer[2])
{
    R = _R;
    G = _G;
    B = _B;
}

RGB24::RGB24(const RGB24 &Pixel)
        : R(m_Buffer[0]),
          G(m_Buffer[1]),
          B(m_Buffer[2])
{
    R = Pixel.R;
    G = Pixel.G;
    B = Pixel.B;
}

RGBA32::RGBA32()
        : R(m_Buffer[0]),
          G(m_Buffer[1]),
          B(m_Buffer[2]),
          A(m_Buffer[3])
{
    R = 0;
    G = 0;
    B = 0;
    A = 0;
}

RGBA32::RGBA32(uint8_t _R, uint8_t _G, uint8_t _B, uint8_t _A)
        : R(m_Buffer[0]),
          G(m_Buffer[1]),
          B(m_Buffer[2]),
          A(m_Buffer[3])
{
    R = _R;
    G = _G;
    B = _B;
    A = _A;
}

RGBA32::RGBA32(const RGBA32 &Pixel)
        : R(m_Buffer[0]),
          G(m_Buffer[1]),
          B(m_Buffer[2]),
          A(m_Buffer[3])
{
    R = Pixel.R;
    G = Pixel.G;
    B = Pixel.B;
    A = Pixel.A;
}

#endif //JIMLIB_PIXELTYPES_H
