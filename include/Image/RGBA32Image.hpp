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

#ifndef JIMLIB_RGBA32IMAGE_HPP
#define JIMLIB_RGBA32IMAGE_HPP

#include "Image/GenericImage.hpp"
#include "Image/PixelTypes.hpp"
namespace jimlib
{
    class RGBA32Image : public GenericImage<PixelType::RGBA32>
    {
    public:
        void CopyFrom(const RGBA32Image &Src);
        void CopyTo(RGBA32Image &Dst) const;
    };

// =======================================================

    inline void RGBA32Image::CopyFrom(const RGBA32Image &Src)
    {
        CopyFromInternal(Src);
    }

    inline void RGBA32Image::CopyTo(RGBA32Image &Dst) const
    {
        CopyToInternal(Dst);
    }
};
#endif //JIMLIB_RGBA32IMAGE_HPP