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

#include "Image/PixelTypes.h"
#include <cassert>
using namespace PixelType;

Mono8::Mono8()
: Value(0)
{
}
Mono8::Mono8(uint8_t _Value)
: Value(_Value)
{
}

Mono8::Mono8(const Mono8 &Pixel)
: Value(Pixel.Value)
{
}

uint8_t &Mono8::operator[] (uint8_t Plant)
{
    assert(Plant == 0);
    return Value;
}

const uint8_t &Mono8::operator[] (uint8_t Plant) const
{
    assert(Plant == 0);
    return Value;
}

Mono16::Mono16()
: Value(0)
{
}

Mono16::Mono16(uint16_t _Value)
: Value(_Value)
{
}

Mono16::Mono16(const Mono16 &Pixel)
: Value(Pixel.Value)
{
}

uint16_t &Mono16::operator[] (uint8_t Plant)
{
    assert(Plant == 0);
    return Value;
}

const uint16_t &Mono16::operator[] (uint8_t Plant) const
{
    assert(Plant == 0);
    return Value;
}

Mono32::Mono32()
: Value(0)
{
}

Mono32::Mono32(uint32_t _Value)
: Value(_Value)
{
}

Mono32::Mono32(const Mono32 &Pixel)
: Value(Pixel.Value)
{
}

uint32_t &Mono32::operator[] (uint8_t Plant)
{
    assert(Plant == 0);
    return Value;
}

const uint32_t &Mono32::operator[] (uint8_t Plant) const
{
    assert(Plant == 0);
    return Value;
}

Mono64::Mono64()
: Value(0)
{
}

Mono64::Mono64(uint64_t _Value)
: Value(_Value)
{
}

Mono64::Mono64(const Mono64 &Pixel)
: Value(Pixel.Value)
{
}

uint64_t &Mono64::operator[] (uint8_t Plant)
{
    assert(Plant == 0);
    return Value;
}

const uint64_t &Mono64::operator[] (uint8_t Plant) const
{
    assert(Plant == 0);
    return Value;
}

RGB24::RGB24()
: R(Value[0]),
  G(Value[1]),
  B(Value[2])
{
    R = 0;
    G = 0;
    B = 0;
}

RGB24::RGB24(uint8_t _R, uint8_t _G, uint8_t _B)
: R(Value[0]),
  G(Value[1]),
  B(Value[2])
{
    R = _R;
    G = _G;
    B = _B;
}

RGB24::RGB24(const RGB24 &Pixel)
: R(Value[0]),
  G(Value[1]),
  B(Value[2])
{
    R = Pixel.R;
    G = Pixel.G;
    B = Pixel.B;
}

uint8_t &RGB24::operator[] (uint8_t Plant)
{
    assert(Plant < 3);
    return Value[Plant];
}

const uint8_t &RGB24::operator[] (uint8_t Plant) const
{
    assert(Plant < 3);
    return Value[Plant];
}

RGBA32::RGBA32()
: R(Value[0]),
  G(Value[1]),
  B(Value[2]),
  A(Value[3])
{
    R = 0;
    G = 0;
    B = 0;
    A = 0;
}

RGBA32::RGBA32(uint8_t _R, uint8_t _G, uint8_t _B, uint8_t _A)
: R(Value[0]),
  G(Value[1]),
  B(Value[2]),
  A(Value[3])
{
    R = _R;
    G = _G;
    B = _B;
    A = _A;
}

RGBA32::RGBA32(const RGBA32 &Pixel)
: R(Value[0]),
  G(Value[1]),
  B(Value[2]),
  A(Value[3])
{
    R = Pixel.R;
    G = Pixel.G;
    B = Pixel.B;
    A = Pixel.A;
}

uint8_t &RGBA32::operator[] (uint8_t Plant)
{
    assert(Plant < 4);
    return Value[Plant];
}

const uint8_t &RGBA32::operator[] (uint8_t Plant) const
{
    assert(Plant < 4);
    return Value[Plant];
}