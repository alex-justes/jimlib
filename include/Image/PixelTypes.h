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

#ifndef PIXELTYPES_H
#define PIXELTYPES_H
#include "GenericPixel.h"
#include <cstdint>

using namespace std;

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
        virtual uint8_t &operator[] (uint8_t Plant) override;
        virtual const uint8_t &operator[] (uint8_t Plant) const override;
        uint8_t Value;
    };

    class Mono16 : public GenericPixel<uint16_t, 1>
    {
    public:
        Mono16();
        Mono16(uint16_t Value);
        Mono16(const Mono16 &Pixel);
        virtual uint16_t &operator[] (uint8_t Plant) override;
        virtual const uint16_t &operator[] (uint8_t Plant) const override;
        uint16_t Value;
    };

    class Mono32 : public GenericPixel<uint32_t, 1>
    {
    public:
        Mono32();
        Mono32(uint32_t Value);
        Mono32(const Mono32 &Pixel);
        virtual uint32_t &operator[] (uint8_t Plant) override;
        virtual const uint32_t &operator[] (uint8_t Plant) const override;
        uint32_t Value;
    };

    class Mono64 : public GenericPixel<uint64_t, 1>
    {
    public:
        Mono64();
        Mono64(uint64_t Value);
        Mono64(const Mono64 &Pixel);
        virtual uint64_t &operator[] (uint8_t Plant) override;
        virtual const uint64_t &operator[] (uint8_t Plant) const override;
        uint64_t Value;
    };

    class RGB24 : public GenericPixel<uint8_t, 3>
    {
    public:
        RGB24();
        RGB24(uint8_t R, uint8_t G, uint8_t B);
        RGB24(const RGB24 &Pixel);
        virtual uint8_t &operator[] (uint8_t Plant) override;
        virtual const uint8_t &operator[] (uint8_t Plant) const override;
        uint8_t &R;
        uint8_t &G;
        uint8_t &B;
    private:
        uint8_t Value[3];
    };

    class RGBA32 : public GenericPixel<uint8_t, 4>
    {
    public:
        RGBA32();
        RGBA32(uint8_t R, uint8_t G, uint8_t B, uint8_t A);
        RGBA32(const RGBA32 &Pixel);
        virtual uint8_t &operator[] (uint8_t Plant) override;
        virtual const uint8_t &operator[] (uint8_t Plant) const override;
        uint8_t &R;
        uint8_t &G;
        uint8_t &B;
        uint8_t &A;
    private:
        uint8_t Value[4];
    };
}

#endif //PIXELTYPES_H
